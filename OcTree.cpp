#include "stdafx.h"
#include "OcTree.h"
#include "scene.h"

const int MAX_DEPTH = 10; 
const int MIN_PRIMITIVE_NUM = 8;

////////////////////////////////////////////////////////////////
// 建立八叉树
// pScene	- 场景指针
////////////////////////////////////////////////////////////////
void Octree::Build( const Scene *pScene )
{
	m_pRoot = new OctreeNode();
	if( NULL != pScene && pScene->Primitives.size() > 0 )
	{
		vector<int> prims;
		for( size_t i=0; i<pScene->Primitives.size(); i++ )
		{
			prims.push_back( i );
		}
		Subdivide( pScene, m_pRoot, pScene->BoundingBox, 0, prims );
	}
}

////////////////////////////////////////////////////////////////
// 递归建立子树
// pScene	- 场景指针
// pNode	- 树结点
// box		- 包围盒
// nDepth	- 递归深度
// prims	- 图元索引集合
////////////////////////////////////////////////////////////////
void Octree::Subdivide( const Scene *pScene, OctreeNode *pNode, const AABB &box, int nDepth, const vector<int> &prims )
{
	pNode->m_Box = box;
	if( ++nDepth >= MAX_DEPTH || prims.size() <= MIN_PRIMITIVE_NUM )
	{
		if( prims.size() > 0 )
		{
			pNode->m_pData = new int[prims.size()];
			for( int i=0; i<(int)prims.size(); i++ ) pNode->m_pData[i] = prims[i];
			pNode->m_nDataNum = (short)prims.size();
		}
		pNode->m_bLeaf = true;
		return;
	}
	AABB b[8];
	double dx = box.w() / 2.0;
	double dy = box.h() / 2.0;
	double dz = box.d() / 2.0;
	
	for( int k=0; k<2; k++ )
	for( int j=0; j<2; j++ )
	for( int i=0; i<2; i++ )
	{
		b[k*4+j*2+i].p1 = box.p1 + Vector3D( dx * i, dy * j, dz * k );
		b[k*4+j*2+i].p2 = b[k*4+j*2+i].p1 + Vector3D( dx, dy, dz );
	}

	vector<int> pri[8];
	for( int k=0; k<(int)prims.size(); k++ )
	for( int i=0; i<8; i++ )
	{
		if( pScene->IntersectPrimitiveAABB( prims[k], b[i] ) )
			pri[i].push_back( prims[k] );
	}
	
	for( int i=0; i<8; i++ )
	{
		if( pri[i].size() > 0 )
		{
			pNode->m_pChild[i] = new OctreeNode();
			Subdivide( pScene, pNode->m_pChild[i], b[i], nDepth, pri[i] );
		}
	}

}

/////////////////////////////////////////////////////////////////////////
// 利用八叉树寻找最近相交点
// pScene				- 场景
// r					- 光线
// t					- 当前最近距离
// intersectNormal		- 相交点的方向
// intersectMaterial	- 相交点的材质
/////////////////////////////////////////////////////////////////////////
bool Octree::FindNearest( const Scene *pScene, const Ray &r, double &t, Normal &intersectNormal, Material** intersectMaterial)
{
	return FindNearestNode( m_pRoot, pScene, r, t, intersectNormal, intersectMaterial);
}

bool Octree::FindNearestNode( const OctreeNode *pNode, const Scene *pScene, const Ray &r, 
							   double &t, Normal &intersectNormal, Material** intersectMaterial)
{
	if( NULL == pNode ) return false;
	
	Ray ray = r;
	AABB box = pNode->m_Box;
	double tMin = 0, tMax = 0;
	if( !box.ContainPoint( ray.o ) )
	{
		if( !RayIntersectAABB( ray, pNode->m_Box, tMin, tMax ) )
			return false;
		else ray.o = ray.o + ray.d * tMin;
	}
	
	bool ret = false;


	if( pNode->m_bLeaf )
	{
		double tTmp = DBL_MAX;
		Normal nrmTmp;
		Material* pMatTmp = NULL;
		for( int i=0; i<pNode->m_nDataNum; i++ )
		{
            
			if( pScene->IntersectPrimitiveRay( pNode->m_pData[i], r, tTmp, nrmTmp, &pMatTmp ) )
			{
				ret = true;
			}
		}

		if( ret )
		{
			Vector3D vi = r.o + r.d * tTmp;
			if( !box.ContainPoint(vi) ) return false;   
		
			t = tTmp;// + tMin;
			intersectNormal = nrmTmp;
			intersectNormal.Normalize();
			*intersectMaterial = pMatTmp;
		}
		return ret;
	}
	
	Vector3D o = ray.o;
	Vector3D d = ray.d;
	double boxW = box.w(), boxH = box.h(), boxD = box.d();
	Vector3D size( boxW * 0.5, boxH * 0.5, boxD * 0.5 ), sizeInv;
	sizeInv.x = 2.0 / boxW;
	sizeInv.y = 2.0 / boxH;
	sizeInv.z = 2.0 / boxD;
	Vector3D cb, tmax, dt, cell = ( o + d * EPSILON - box.p1 ) * sizeInv;

	int stepX, outX, X = (int)cell.x;
	int stepY, outY, Y = (int)cell.y;
	int stepZ, outZ, Z = (int)cell.z;
	
	if( !(X > -1 && X < 2 && Y > -1 && Y < 2 && Z > -1 && Z < 2) )
	{
		return false;
	}

	if( d.x > 0 ) {stepX = 1; outX = 2; cb.x = box.p1.x + (X+1) * size.x;}
	else { stepX = -1; outX = -1; cb.x = box.p1.x + (X) * size.x; }
	if( d.y > 0 ) {stepY = 1; outY = 2; cb.y = box.p1.y + (Y+1) * size.y;}
	else { stepY = -1; outY = -1; cb.y = box.p1.y + (Y) * size.y; }
	if( d.z > 0 ) {stepZ = 1; outZ = 2; cb.z = box.p1.z + (Z+1) * size.z;}
	else { stepZ = -1; outZ = -1; cb.z = box.p1.z + (Z) * size.z; }

	double rxr, ryr, rzr;
	if( d.x != 0 ) { rxr = 1.0 / d.x; tmax.x = (cb.x - o.x ) * rxr; dt.x = size.x * stepX * rxr; }
	else tmax.x = FLT_MAX;
	if( d.y !=0 ) { ryr = 1.0 / d.y; tmax.y = (cb.y - o.y ) * ryr; dt.y = size.y * stepY * ryr; }
	else tmax.y = FLT_MAX;
	if( d.z !=0 ) { rzr = 1.0 / d.z; tmax.z = (cb.z - o.z ) * rzr; dt.z = size.z * stepZ * rzr; }
	else tmax.z = FLT_MAX;

	while( true )
	{
		if( FindNearestNode( pNode->m_pChild[ X + 2 * Y + 4 * Z ], pScene, r, t, intersectNormal,  intersectMaterial ) ) 
			return true;
		if( tmax.x < tmax.y ) 
		{
			if( tmax.x < tmax.z )
			{
				X = X + stepX;
				if( X == outX ) return false;
				tmax.x += dt.x;
			}
			else
			{
				Z = Z + stepZ;
				if (Z == outZ) return false;
				tmax.z += dt.z;
			}
		}
		else
		{
			if( tmax.y < tmax.z )
			{
				Y = Y + stepY;
				if( Y == outY ) return false;
				tmax.y += dt.y;
			}
			else
			{
				Z = Z + stepZ;
				if (Z == outZ) return false;
				tmax.z += dt.z;
			}
		}
	}
}
