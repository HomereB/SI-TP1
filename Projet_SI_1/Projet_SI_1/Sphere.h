#pragma once
#include "Vector.h"
#include "Ray.h""

class Sphere
{
public : 
	Vec3<float> center;
	float ray;

	Sphere(Vec3<float> position, float rayon)
	{
		center = position;
		ray = rayon;
	}
};