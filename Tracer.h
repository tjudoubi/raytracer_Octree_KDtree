#ifndef _TRACER_H
#define _TRACER_H

#include "Vector3D.h"
#include "Scene.h"
#include "Camera.h"


#define SUPER_SAMPLE_X 4
#define SUPER_SAMPLE_Y 4

typedef unsigned int Pixel;

class Tracer
{
public:
	Tracer();
	~Tracer();
	void SetTarget(Camera* a_camera, Scene* a_scene, Pixel* a_Dest, int a_Width, int a_Height,int a_Depth );
	bool RayCast(bool bAntialias, bool bAcceleration);
	int GetRLine ()const { return m_iCurrLine;}
protected:
	void traceRay( Ray& a_Ray, Color& a_Acc, int a_Depth, double rIndexIn, bool bAcceleration);
	void ConstructRayThroughPixel(double x, double y, Ray* ray);
	void Shade(const Point& a_point, const Normal& a_normal, Material* a_material, Color& accColor, bool bAcceleration);
	int computeShadowAttenuation(const Point& light, const Point& vertex, bool bAcceleration);
	Vector3D reflectDirection(const Normal& vNormal, const Vector3D& vIncident);
	Vector3D refractDirection(const Normal& vNormal, const Vector3D& vIncident, double rIndexIn, double rIndexOut);
	// renderer data
	double m_fDiffX, m_fDiffY;
	Scene* m_pScene;
	Camera* m_pCamera;
	Pixel* m_pDest;
	int m_iWidth, m_iHeight, m_iCurrLine;
	int m_iDepth;
};
#endif