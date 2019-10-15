#pragma once
#include "Vector.h"
#include "Ray.h"

class Sphere
{
public : 
	Vec3<float> center;
	float ray;
	Vec3<float> color;
	float albedo;

	Sphere(Vec3<float> position, float rayon,Vec3<float> col,float a)
	{
		center = position;
		ray = rayon;
		color = col;
		albedo = a;
	}
};