#include "stdafx.h"
#include "Tracer.h"

Tracer::Tracer()
{

}

Tracer::~Tracer()
{

}

// -----------------------------------------------------------
// Tracer::SetTarget
// Sets the render target canvas
// -----------------------------------------------------------
void Tracer::SetTarget(Camera* a_camera, Scene* a_scene, Pixel* a_Dest, int a_Width, int a_Height, int a_Depth)
{
	// set pixel buffer address & size
	m_pCamera = a_camera;
	m_pScene = a_scene;
	m_pDest = a_Dest;
	m_iWidth = a_Width;
	m_iHeight = a_Height;
	m_iDepth = a_Depth;

	m_iCurrLine = 0;

	Vector3D f = m_pCamera->center - m_pCamera->position;

	// step for each pixel
	m_fDiffX = 2 * tan(deg2rad(m_pCamera->fovy)*m_pCamera->aspect / 2.0)*f.Length() / m_iWidth;
	m_fDiffY = 2 * tan(deg2rad(m_pCamera->fovy) / 2.0)*f.Length() / m_iHeight;
}


void Tracer::traceRay(Ray& a_Ray, Color& a_Acc, int a_Depth, double rIndexIn, bool bAcceleration)
{
	bool bIntersect = false;

	Point intersectPos;
	Normal intersectNormal;
	Material* intersectMaterial;

	double dist = DBL_MAX;

	bIntersect = m_pScene->FindIntersection(a_Ray, dist, intersectNormal, &intersectMaterial, bAcceleration);

	if (bIntersect)
	{
		intersectPos = a_Ray(dist);
		intersectNormal.Normalize();
		Shade(intersectPos, intersectNormal, intersectMaterial, a_Acc, bAcceleration);

		if (a_Depth < m_iDepth)
		{
			Color reflectColor;
			Color refractColor;

			if (intersectMaterial->reflect > 0.0) {

				Vector3D reflectDir = reflectDirection(intersectNormal, a_Ray.d);
				Ray reflectRay(intersectPos + reflectDir * EPSILON, reflectDir);

				traceRay(reflectRay, reflectColor, a_Depth + 1, rIndexIn, bAcceleration);
			}

			if (intersectMaterial->transparent > 0.0) {

				double n_i, n_t;
				if (rIndexIn != REFRACTIVE_INDEX_AIR)
				{
					n_i = rIndexIn;
					n_t = REFRACTIVE_INDEX_AIR;
				}
				else
				{
					n_i = REFRACTIVE_INDEX_AIR;
					n_t = intersectMaterial->refractiveIndex;
				}

				double thetaC = asin(n_t / n_i);
				double thetaI = acos(-Dot(a_Ray.d, intersectNormal));

				if (thetaI < thetaC)
				{

					Vector3D refractDir = refractDirection(intersectNormal, a_Ray.d, n_i, n_t);
					Ray refractRay(intersectPos + refractDir * EPSILON, refractDir);

					traceRay(refractRay, refractColor, a_Depth + 1, n_t, bAcceleration);
				}
			}

			//// combine reflection/refraction ray colours with original ray colours
			double r = intersectMaterial->reflect;
			double t = intersectMaterial->transparent;
			double l = 1.0 - r - t;

			a_Acc = a_Acc * l + reflectColor * r + refractColor * t;
		}
	}
}

// -----------------------------------------------------------
// Tracer::Render
// Fires rays in the scene one scanline at a time, from left
// to right
// -----------------------------------------------------------
bool Tracer::RayCast(bool bAntialias, bool bAcceleration)
{
	Ray r;
	int msecs = GetTickCount();

	CString str;
	str.Format(_T("Start %d\n"), m_iCurrLine);
	TRACE(str);

	for (int y = m_iCurrLine; y < m_iHeight; y++)
	{
		for (int x = 0; x < m_iWidth; x++)
		{
			//printf("X: %d, Y: %d\n", x, y);
			Color acc(0, 0, 0);
			if (!bAntialias)
			{
				ConstructRayThroughPixel(x, y, &r);
				traceRay(r, acc, 1, REFRACTIVE_INDEX_AIR, bAcceleration);
			}
			else
			{
				for (int i = 0; i < SUPER_SAMPLE_Y; i++)
				{
					for (int j = 0; j < SUPER_SAMPLE_X; j++)
					{
						Color subColor;
						double subX = x + (j + 0.5) / SUPER_SAMPLE_X - 0.5;
						double subY = y + (i + 0.5) / SUPER_SAMPLE_Y - 0.5;
						ConstructRayThroughPixel(subX, subY, &r);
						traceRay(r, subColor, 1, REFRACTIVE_INDEX_AIR, bAcceleration);

						acc += subColor;
					}
				}
				acc /= SUPER_SAMPLE_X * SUPER_SAMPLE_Y;
			}
			clamp(&acc.r);
			clamp(&acc.g);
			clamp(&acc.b);
			int red = acc.r * 255;
			int green = acc.g * 255;
			int blue = acc.b * 255;

			m_pDest[(m_iHeight - y - 1) * m_iWidth + x] = (red << 16) + (green << 8) + blue;
		}

		if ((GetTickCount() - msecs) > 100)
		{
			m_iCurrLine = y + 1;

			str.Format(_T("End %d %d\n"), y, m_iCurrLine);
			TRACE(str);

			return false;
		}
	}
	return true;
}

void Tracer::ConstructRayThroughPixel(double x, double y, Ray* ray)
{
	double alpha = x - m_iWidth / 2 + 0.5;
	double beta = y - m_iHeight / 2 + 0.5;

	// 像素点的世界坐标
	Vector3D pRayLookAt = m_pCamera->center + alpha * m_fDiffX * m_pCamera->right + beta * m_fDiffY * m_pCamera->up;


	// 光线的方向
	Vector3D vRayDirection = pRayLookAt - m_pCamera->position;
	vRayDirection.Normalize();

	// ray object
	(*ray).o = m_pCamera->position;
	(*ray).d = vRayDirection;
}

void Tracer::Shade(const Point& vVertex, const Normal& vNormal, Material* material, Color& accColor, bool bAcceleration)
{
	accColor = material->emission;

	for (size_t i = 0; i < m_pScene->Lights.size(); i++)
	{
		Light* light = m_pScene->Lights[i];

		Color color;
		Color ambColor;
		Color diffColor;
		Color specColor;

		// relative light & camera direction vectors
		Vector3D vRawVertexToLight = light->position - vVertex;
		Vector3D vRawVertexToCamera = m_pCamera->position - vVertex;

		// normalized light & camera direction vectors
		Vector3D vNormVertexToLight = vRawVertexToLight;
		vNormVertexToLight.Normalize();
		Vector3D vNormVertexToCamera = vRawVertexToCamera;
		vNormVertexToCamera.Normalize();

		// distance from vertex to light
		double distance = vRawVertexToLight.Length();

		// half-vector between camera and light
		Vector3D vHalf = (vNormVertexToLight + vNormVertexToCamera) * 0.5;
		vHalf.Normalize();

		// 光源衰减因子
		double attenuation = 1 /
			(light->attenuation[0] + light->attenuation[1] * distance + light->attenuation[2] * distance*distance);

		// 环境光
		ambColor = light->ambient * material->ambient;

		// 漫反射光
		double diffuse = Dot(vNormal, vNormVertexToLight);
		if (diffuse > 0.0) {
			diffuse *= attenuation;
			diffColor = diffuse * light->diffuse * material->diffuse;
		}

		// 镜面光
		double specular = pow(Dot(vNormal, vHalf), material->shininess);
		if (specular > 0.0) {
			specular *= attenuation;
			specColor = specular * light->specular * material->specular;
		}

		color = ambColor + diffColor + specColor;

		/////////////////////////////////////////////////////////////////////////////////////
		// 阴影测试

		int Ashadow;
		if (Dot(vNormal, vNormVertexToLight) <= 0.0) {
			Ashadow = 0;
		}
		else {
			Ashadow = computeShadowAttenuation(light->position, vVertex, bAcceleration);
		}
		color *= Ashadow;

		/////////////////////////////////////////////////////////////////////////////////////

		accColor += color;
	}
}

int Tracer::computeShadowAttenuation(const Point& light, const Point& vertex, bool bAcceleration)
{
	Vector3D vVertexToLight = light - vertex;
	double tdist = vVertexToLight.Length();
	vVertexToLight.Normalize();
	Ray shadowRay = Ray(vertex + vVertexToLight * EPSILON, vVertexToLight);
	Normal tmpNormal;
	Material* tmpMaterial;
	if (m_pScene->FindIntersection(shadowRay, tdist, tmpNormal, &tmpMaterial, bAcceleration))
		return 0;
	else
		return 1;

}

Vector3D Tracer::reflectDirection(const Normal& vNormal, const Vector3D& vIncident)
{
	Vector3D vR = vIncident - 2 * Dot(vNormal, vIncident) * vNormal;
	vR.Normalize();
	return vR;
}

Vector3D Tracer::refractDirection(const Normal& vNormal, const Vector3D& vIncident, double rIndexIn, double rIndexOut)
{
	double r = rIndexIn / rIndexOut;
	double c1 = -Dot(vNormal, vIncident);
	double c2 = sqrt(1 - (r*r) * (1 - (c1*c1)));
	Vector3D vR = (vIncident * r) + (vNormal * (r * c1 - c2));
	vR.Normalize();

	return vR;
}