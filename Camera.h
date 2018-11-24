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

	//���λ��
	Point position;
	//���ָ���λ��,����ƽ������λ��
	Point center;
	// up vector
	Vector3D up;
	// right vector
	Vector3D right;

	// ��ֱ����field of view
	double fovy;
	// ��߱�
	double aspect;
};

#endif