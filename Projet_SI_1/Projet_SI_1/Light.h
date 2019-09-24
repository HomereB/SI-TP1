#pragma once
#include "Vector.h"
#include "FreeImage.h"


class Light
{
public:
	Vec3<float> pos;
	Vec3<float> color;
	int intensity;

	Light(Vec3<float> position, Vec3<float> col,int i)
	{
		pos = position;
		color = col;
		intensity = i;
	}
};