#ifndef _MATERIAL_H
#define _MATERIAL_H

#define REFRACTIVE_INDEX_AIR 1.0003

class Material
{
	
public:
	Material():specular(1.0, 1.0, 1.0),shininess(500.0),reflect(0.7),transparent(0.0),refractiveIndex(0.0)
	{}

	~Material() {};

	Color ambient;
	Color diffuse;
	Color specular;
	Color emission;

	// shininess factor
	double shininess;
	double reflect;
	double transparent;
	double refractiveIndex;
};

#endif