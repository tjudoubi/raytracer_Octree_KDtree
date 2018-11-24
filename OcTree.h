#ifndef _OCTREE_H
#define _OCTREE_H

#include <vector>
#include "Vector3D.h"
#include "Primitive.h"


using std::vector;

class Scene;
class Material;


struct OctreeNode
{
	OctreeNode() : m_nDataNum(0), m_pData(NULL), m_bLeaf(false)
	{
		for( int i=0; i<8; i++ ) m_pChild[i] = NULL;
	}
	~OctreeNode()
	{
		for( int i=0; i<8; i++ )
			if( NULL != m_pChild[i] ) delete m_pChild[i];
		if( NULL != m_pData ) delete m_pData;
	}

	int *m_pData;
	AABB m_Box;
	OctreeNode *m_pChild[8];
	short m_nDataNum;
	bool m_bLeaf;
};

class Octree
{
public:
	Octree() : m_pRoot(NULL) {}
	~Octree() { if( NULL != m_pRoot ) delete m_pRoot; }
	void Build( const Scene *pScene );
	const OctreeNode *GetRoot() const { return m_pRoot; }
	bool FindNearest(const Scene *pScene, const Ray &r, double &t, Normal &intersectNormal, Material** intersectMaterial);
private:
	bool FindNearestNode( const OctreeNode *pNode, const Scene *pScene, const Ray &r, double &t, Normal &intersectNormal, Material** intersectMaterial);
	void Subdivide(  const Scene *pScene, OctreeNode *pNode, const AABB &box, int nDepth, const vector<int> &prims );
	OctreeNode *m_pRoot;
};

#endif