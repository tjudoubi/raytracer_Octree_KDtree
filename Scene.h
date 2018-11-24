#ifndef _SCENE_H
#define _SCENE_H

#include "Primitive.h"
#include "Matrix.h"
#include "light.h"
#include "KDTree.h"
#include <vector>

class Scene
{
public:
	Scene(){}
	~Scene();
	void InitScene();
	bool IntersectPrimitiveAABB( int priIdx, const AABB &box ) const;
	bool IntersectPrimitiveRay( int priIdx, const Ray& a_ray, double &t, Normal &nrm, Material** pMat) const;
	bool FindIntersection( const Ray& a_ray, double &t, Normal &intersectNormal, Material** intersectMaterial, bool bAcceleration);

private:
	void ComputeBoundingBox(double eps);
	bool IntersectDirectly(const Ray& a_ray, double& t, Normal& intersectNormal, Material** intersectMaterial);
	bool IntersectOctree(const Ray& a_ray, double& t, Normal& intersectNormal, Material** intersectMaterial);
	bool LoadObjMesh( const char* pFileName, Material* mat, const Matrix4x4 &mtx);

public:
	std::vector<Primitive*> Primitives;
	std::vector<Light*> Lights;
	std::vector<Material*> Materials;
	AABB BoundingBox;
	KDTree *kdtree;
	
};
#endif