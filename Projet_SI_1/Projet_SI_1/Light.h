#pragma once
#include "Vector.h"
#include "FreeImage.h"


class Light
{
public:
	Vec3<float> pos;
	RGBQUAD color;
	int intensity;

	Light(Vec3<float> position, RGBQUAD col,int i)
	{
		pos = position;
		color = col;
		intensity = i;
	}
};