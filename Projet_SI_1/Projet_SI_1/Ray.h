#pragma once
#include "Vector.h"

class Ray
{
public:
	Vec3<float> pos;
	Vec3<float> dir;

	Ray(Vec3<float> position, Vec3<float> direction)
	{
		pos = position;
		dir = direction;
	}
};