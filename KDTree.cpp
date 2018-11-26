#include "stdafx.h"
#include "KDTree.h"
#include "scene.h"
#include <iostream>
using namespace std;
const int MAX_DEPTH = 100;
const int MIN_PRIMITIVE_NUM = 8;


void KDTree::Build(const Scene *pScene)
{
	m_pRoot = new kdtreeNode();
	if (NULL != pScene && pScene->Primitives.size() > 0)
	{
		vector<int> prims;
		m_pRoot->level = 0;
		for (size_t i = 0; i < pScene->Primitives.size(); i++)
		{
			prims.push_back(i);
		}
		divide(pScene, m_pRoot, pScene->BoundingBox, prims,0);
	}
}

void KDTree::divide(const Scene *pScene, kdtreeNode *pNode, const AABB &box, const vector<int> &prims,int nDepth)
{
	pNode->m_Box = box;
	if (nDepth+1 >= MAX_DEPTH || prims.size() <= MIN_PRIMITIVE_NUM)
	{
		if (prims.size() > 0)
		{
			pNode->m_pData = new int[prims.size()];
			for (int i = 0; i < (int)prims.size(); i++) pNode->m_pData[i] = prims[i];
			pNode->m_nDataNum = (short)prims.size();
		}
		pNode->m_bLeaf = true;
		return ;
	}
	
		AABB b[2];//////////2个分支
		double dx = box.w() / 2.0;
		double dy = box.h() / 2.0;
		double dz = box.d() / 2.0;

		//////////根据深度确定划定切分轴
		if (nDepth % 3 == 0)
		{
			for (int k = 0; k < 2; k++)//////////////每个分支做一个轴对称绑定盒
			{
				b[k].p1 = box.p1 + Vector3D(dx * k, 0, 0);/////每个分支的下界
				b[k].p2 = box.p2 - Vector3D(dx*(1 - k), 0, 0);///////每个分支的上界
			}
		}
		else if (nDepth % 3 == 1) {
			for (int k = 0; k < 2; k++)//////////////每个分支做一个轴对称绑定盒
			{
				b[k].p1 = box.p1 + Vector3D(0, dy*k, 0);/////每个分支的下界
				b[k].p2 = box.p2 - Vector3D(0, dy*(1-k), 0);///////每个分支的上界
			}
		}
		else if (nDepth % 3 == 2) {
			for (int k = 0; k < 2; k++)//////////////每个分支做一个轴对称绑定盒
			{
				b[k].p1 = box.p1 + Vector3D(0, 0, dz*k);/////每个分支的下界
				b[k].p2 = box.p2 - Vector3D(0, 0, dz*(1-k));///////每个分支的上界

				cout << k << " " << b[k].p1.x << " " << b[k].p2.x << endl;
			}
		}

		vector<int> pri[2];/////分叉集合
		for (int k = 0; k < (int)prims.size(); k++) {
			for (int i = 0; i < 2; i++)
			{
				if (pScene->IntersectPrimitiveAABB(prims[k], b[i]))
					pri[i].push_back(prims[k]);
			}
		}

		if (pri[0].size() > 0) {
			pNode->m_pChild[0] = new kdtreeNode();
			pNode->level = nDepth+1;
			divide(pScene, pNode->m_pChild[0], b[0], pri[0],nDepth+1);
		}
		if (pri[1].size() > 0) {
			pNode->m_pChild[1] = new kdtreeNode();
			pNode->level = nDepth+1;
			divide(pScene, pNode->m_pChild[1], b[1], pri[1], nDepth + 1);
		}

	
}

bool KDTree::FindNearest(const Scene *pScene, const Ray &r, double &t, Normal &intersectNormal, Material** intersectMaterial)
{
	return FindNearestNode(m_pRoot, pScene, r, t, intersectNormal, intersectMaterial);
}

bool KDTree::FindNearestNode(const kdtreeNode *pNode, const Scene *pScene, const Ray &r,
	double &t, Normal &intersectNormal, Material** intersectMaterial)
{
	if (NULL == pNode) return false;

	Ray ray = r;
	AABB box = pNode->m_Box;
	double tMin = 0, tMax = 0;
	if (!box.ContainPoint(ray.o))
	{
		if (!RayIntersectAABB(ray, pNode->m_Box, tMin, tMax))
			return false;
		else ray.o = ray.o + ray.d * tMin;
	}

	bool ret = false;


	if (pNode->m_bLeaf)
	{
		double tTmp = DBL_MAX;
		Normal nrmTmp;
		Material* pMatTmp = NULL;
		for (int i = 0; i < pNode->m_nDataNum; i++)
		{

			if (pScene->IntersectPrimitiveRay(pNode->m_pData[i], r, tTmp, nrmTmp, &pMatTmp))
			{
				ret = true;
			}
		}

		if (ret)
		{
			Vector3D vi = r.o + r.d * tTmp;
			if (!box.ContainPoint(vi)) return false;

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
	Vector3D size(boxW * 0.5, boxH * 0.5, boxD * 0.5), sizeInv;
	sizeInv.x = 2.0 / boxW;
	sizeInv.y = 2.0 / boxH;
	sizeInv.z = 2.0 / boxD;
	Vector3D cb, tmax, dt, cell = (o + d * EPSILON - box.p1) * sizeInv;

	int stepX, outX, X = (int)cell.x;
	int stepY, outY, Y = (int)cell.y;
	int stepZ, outZ, Z = (int)cell.z;

	if (!(X > -1 && X < 2 && Y > -1 && Y < 2 && Z > -1 && Z < 2))
	{
		return false;
	}

	if (d.x > 0) { stepX = 1; outX = 2; cb.x = box.p1.x + (X + 1) * size.x; }
	else { stepX = -1; outX = -1; cb.x = box.p1.x + (X)* size.x; }
	if (d.y > 0) { stepY = 1; outY = 2; cb.y = box.p1.y + (Y + 1) * size.y; }
	else { stepY = -1; outY = -1; cb.y = box.p1.y + (Y)* size.y; }
	if (d.z > 0) { stepZ = 1; outZ = 2; cb.z = box.p1.z + (Z + 1) * size.z; }
	else { stepZ = -1; outZ = -1; cb.z = box.p1.z + (Z)* size.z; }

	double rxr, ryr, rzr;
	if (d.x != 0) { rxr = 1.0 / d.x; tmax.x = (cb.x - o.x) * rxr; dt.x = size.x * stepX * rxr; }
	else tmax.x = FLT_MAX;
	if (d.y != 0) { ryr = 1.0 / d.y; tmax.y = (cb.y - o.y) * ryr; dt.y = size.y * stepY * ryr; }
	else tmax.y = FLT_MAX;
	if (d.z != 0) { rzr = 1.0 / d.z; tmax.z = (cb.z - o.z) * rzr; dt.z = size.z * stepZ * rzr; }
	else tmax.z = FLT_MAX;


	int one = 0, zero = 0;
	while (true)
	{
		///////////////////////////////////启发式搜索
		int pChild = 0;
		bool judge = false;

		/*if (pNode->level % 3 == 0) { ////讲道理，我觉得这样判断才比较合理，然而使用这段代码并不能渲染出场景。。。。。。。
			if (X == 0) {
				zero++;
				pChild = 0;
			}
			else {
				one++;
				pChild = 1;
			}
		}
		else if (pNode->level % 3 == 1) {
			if (Y == 0) {
				zero++;
				pChild = 0;
			}
			else {
				one++;
				pChild = 1;
			}
		}
		else if (pNode->level % 3 == 0) {
			if (Z == 0) {
				zero++;
				pChild = 0;
			}
			else {
				one++;
				pChild = 1;
			}
		}*/


		//////////////////////////////////////////////////我也不知道为什么需要这么判断，但只有这么判断才能渲染出场景，玄学
		if (pNode->level % 3 == 0) {
			if (Z == 0) {
				zero++;
				pChild = 0;
			}
			else {
				one++;
				pChild = 1;
			}
		}
		else if (pNode->level % 3 == 1) {
			if (X == 0) {
				zero++;
				pChild = 0;
			}
			else {
				one++;
				pChild = 1;
			}
		}
		else if (pNode->level % 3 == 2) {
			if (Y == 0) {
				zero++;
				pChild = 0;
			}
			else {
				one++;
				pChild = 1;
			}
		}
		if (pChild == 0 && zero <= 1) { judge = true; }
		else if (pChild == 1 && one <= 1) { judge = true; }

		
		if (judge&&FindNearestNode(pNode->m_pChild[pChild], pScene, r, t, intersectNormal, intersectMaterial))
			return true;
		

		/*if (tmax.x < tmax.y)  //////使用这段代码 当ndepth 和 size 调至最大时，渲染场景时间大约需要2.5s 用下面未注释的代码约为2.6s 然而我还是觉得下面的代码更合理
		{
			if (tmax.x < tmax.z)
			{
				X = X + stepX;
				if (X == outX) return false;
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
			if (tmax.y < tmax.z)
			{
				Y = Y + stepY;
				if (Y == outY) return false;
				tmax.y += dt.y;
			}
			else
			{
				Z = Z + stepZ;
				if (Z == outZ) return false;
				tmax.z += dt.z;
			}
		}*/
		///////////////////////////////////////////////////////////这段代码 depth 和 size 调至最大值，渲染时间约为2.6s
		if (pNode->level % 3 == 0) {
			Z = Z + stepZ;
			if (Z == outZ) return false;
		}
		else if (pNode->level % 3 == 1) {
			X = X + stepX;
			if (X == outX) return false;
		}
		else if (pNode->level % 3 == 2) {
			Y = Y + stepY;
			if (Y == outY) return false;
		}
	}
}
