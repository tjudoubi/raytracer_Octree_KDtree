#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vector3D.h"

class Camera
{
public:
	Camera(const Point& _pos, const Point& _lookat, const Vector3D& _up):position(_pos), center(_lookat)
	{
		up = _up;
		up.Normalize();

		Vector3D f = center - position;
		right = Cross(f, up);
		right.Normalize();
	}
	~Camera() {};

	//相机位置
	Point position;
	//相机指向的位置,即像平面中心位置
	Point center;
	// up vector
	Vector3D up;
	// right vector
	Vector3D right;

	// 垂直方向field of view
	double fovy;
	// 宽高比
	double aspect;
};

#endif