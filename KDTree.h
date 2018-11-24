#ifndef _KDTREE_H
#define _KDTREE_H

#include "Vector3D.h"
#include <vector>
#include "Primitive.h"

using std::vector;

class Scene;
class Material;


struct kdtreeNode
{
	kdtreeNode():m_nDataNum(0), m_pData(NULL),
	m_bLeaf(false)
	{
		for (int i = 0; i < 2; i++) m_pChild[i] = NULL;
	}
	~kdtreeNode()
	{
		if(m_pChild[0] != NULL) delete m_pChild[0];
		if(m_pChild[1] != NULL) delete m_pChild[1];
	}
	int *m_pData;
	AABB m_Box;
	kdtreeNode* m_pChild[2];////0 left , 1 right
	bool m_bLeaf;
	short m_nDataNum;
	int level;
};

class KDTree
{
public:
	KDTree() : m_pRoot(NULL) {}
	~KDTree() { if (NULL != m_pRoot) delete m_pRoot; }
	const kdtreeNode *GetRoot() const { return m_pRoot; }
	kdtreeNode *m_pRoot;
	void Build( const Scene *pScene );
	void divide(const Scene * pScene, kdtreeNode * pNode, const AABB & box, const vector<int>& prims, int nDepth);
	bool FindNearest(const Scene *pScene, const Ray &r, double &t, Normal &intersectNormal, Material** intersectMaterial);
	bool FindNearestNode(const kdtreeNode * pNode, const Scene * pScene, const Ray & r, double & t, Normal & intersectNormal, Material ** intersectMaterial);



};

#endif