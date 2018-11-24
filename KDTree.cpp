#include "stdafx.h"
#include "KDTree.h"
#include "scene.h"
#include <iostream>
using namespace std;
const int MAX_DEPTH = 1000;
const int MIN_PRIMITIVE_NUM = 10;


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
	if (nDepth >= MAX_DEPTH || prims.size() <= MIN_PRIMITIVE_NUM)
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
	cout << pNode->m_Box.p1.x << endl;
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



	int i = 2;
	while (i > 0)
	{
		i--;
		if (FindNearestNode(pNode->m_pChild[i], pScene, r, t, intersectNormal, intersectMaterial))
			return true;
	}
	return false;
}
