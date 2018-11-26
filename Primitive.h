#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "Vector3D.h"
#include "Material.h"


struct AABB
{
	Vector3D p1; // 000
	Vector3D p2; // 111

	double w() const { return p2.x - p1.x; }
	double h() const { return p2.y - p1.y; }
	double d() const { return p2.z - p1.z; }
	bool ContainPoint( const Vector3D &p ) const
	{
		return ( p.x >= p1.x &&  p.x <= p2.x &&  
				 p.y >= p1.y &&  p.y <= p2.y &&
				 p.z >= p1.z &&  p.z <= p2.z );
	}
	bool Intersect( const AABB &b2 ) const
	{
		Vector3D v1 = b2.p1, v2 = b2.p2;

		return ( (p2.x > v1.x) && (p1.x < v2.x) && // x-axis overlap
				 (p2.y > v1.y) && (p1.y < v2.y) && // y-axis overlap
				 (p2.z > v1.z) && (p1.z < v2.z) );   // z-axis overlap

	}
};

bool RayIntersectAABB( const Ray &r, const AABB &box, double &tMin, double &tMax );

enum PRIMITIVE_TYPE
{
	SPHERE,
	PLANE,
	TRIANGLE,
};

class Primitive
{
public:
	Primitive(){}
	Material* GetMaterial() { return m_Material; }
	void SetMaterial(Material* a_Mat ) { m_Material = a_Mat; }
	virtual bool Intersect(const Ray& a_Ray, double& a_Dist ) = 0;
	virtual Normal GetNormal(const Point& a_Pos ) = 0;
	virtual bool IntersectAABB(const AABB &box) = 0;
	virtual int GetType()const=0;
protected:
	Material* m_Material;
};

class Sphere : public Primitive
{
public:
	Sphere(double a_Radius, const Point& a_Centre) : 
		Center(a_Centre), SqRadius(a_Radius * a_Radius), Radius(a_Radius), RRadius(1.0f / a_Radius)
	{
	}
	bool Intersect(const Ray& a_Ray, double& a_Dist );
	bool IntersectAABB(const AABB &box);
	Normal GetNormal(const Point& a_Pos ) { return (a_Pos - Center) * RRadius; }
	int GetType()const{return SPHERE;}

public:
	Point Center;
	double SqRadius, Radius, RRadius;
};

class Triangle:public Primitive
{
public:
	Triangle():v0(0, 0, 0), v1(0, 0, 1), v2(1, 0, 0), normal(0, 1, 0) {}
	Triangle(const Point& _pt1, const Point& _pt2, const Point& _pt3):v0(_pt1), v1(_pt2), v2(_pt3)
	{
		normal = Cross(v1-v0, v2-v0);
		normal.Normalize();
	}

	bool IntersectAABB(const AABB &box);
	bool Intersect(const Ray& a_Ray, double& a_Dist );
	Normal GetNormal(const Point& a_Pos ){return normal;}
	int GetType()const{return TRIANGLE;}

public:
	Point v0;
	Point v1;
	Point v2;
	Normal normal;

};


#endif
