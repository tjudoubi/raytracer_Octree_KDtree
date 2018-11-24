#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include<cassert>
#include<cmath>
#include<cfloat>

#define PI 3.14159265358979323846
#define EPSILON 0.000001f

class Vector3D
{
public:
	Vector3D(double _x=0, double _y=0, double _z=0)
		: x(_x), y(_y), z(_z) 
	{
	}
	Vector3D(const Vector3D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vector3D operator+(const Vector3D &v) const {
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}
	
	Vector3D& operator+=(const Vector3D &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	Vector3D operator-(const Vector3D &v) const {
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}
	
	Vector3D& operator-=(const Vector3D &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	bool operator==(const Vector3D &v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	Vector3D operator*(double f) const {
		return Vector3D(f*x, f*y, f*z);
	}
	
	Vector3D &operator*=(double f) {
		x *= f; y *= f; z *= f;
		return *this;
	}

	Vector3D operator/(double f) const {
		assert(f!=0);
		double inv = 1.f / f;
		return Vector3D(x * inv, y * inv, z * inv);
	}
	
	Vector3D &operator/=(double f) {
		assert(f!=0);
		double inv = 1.f / f;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}
	Vector3D operator-() const {
		return Vector3D(-x, -y, -z);
	}
	double operator[](int i) const {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}
	
	double &operator[](int i) {
		assert(i >= 0 && i <= 2);
		return (&x)[i];
	}
	double LengthSquared() const { return x*x + y*y + z*z; }
	double Length() const { return sqrt(LengthSquared()); }
	void Normalize()
	{
		double l=1/Length();
		x *= l;
		y *= l;
		z *= l;
	}
	union
	{
		struct { double x, y, z; };
		struct { double r, g, b; };
		struct { double cell[3]; };
	};
};

inline Vector3D Normalize(const Vector3D &v) {
	return v / v.Length();
}

inline Vector3D operator * (double s, const Vector3D& v)
{
	return Vector3D(s*v.x, s*v.y, s*v.z);
}

inline Vector3D operator*(const Vector3D &v1, const Vector3D& v2){
		return Vector3D(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
	}

inline double Dot(const Vector3D &v1, const Vector3D &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vector3D Cross(const Vector3D &v1, const Vector3D &v2) {
	return Vector3D((v1.y * v2.z) - (v1.z * v2.y),
                  (v1.z * v2.x) - (v1.x * v2.z),
                  (v1.x * v2.y) - (v1.y * v2.x));
}

typedef Vector3D Color;
typedef Vector3D Point;
typedef Vector3D Normal;

class Ray
{
public:
	Ray() {}
	Ray(const Point &origin, const Vector3D &direction)
		: o(origin), d(direction) { }
	Point operator()(double t) const { return o + d * t; }

	// Ray Public Data
	Point o;
	Vector3D d;
};


///////////////////////////////////////////////////////////////////////////

static double rad2deg(double rad) {
	return rad*360.0/(2*PI);
};

static double deg2rad(double deg) {
	return deg*2*PI/360.0;
};

static void clamp(double *val) {
	if (*val < 0.0) {
		*val = 0.0;
	} else if (*val > 1.0) {
		*val = 1.0;
	}
};

///////////////////////////////////////////////////////////////////////////

#endif