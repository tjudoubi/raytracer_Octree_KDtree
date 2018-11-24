#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vector3D.h"
class Light
{
public:
	Light() {};
	// destructor
	~Light() {};

	// position
	Point position;
	// ambient color	
	Color ambient;
	// diffuse color
	Color diffuse;
	// specular color
	Color specular;
	// light attenuation
	double attenuation[3];
};
#endif