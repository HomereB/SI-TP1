#pragma once
#include <vector>
#include "Vector.h"

class Box
{
public:
	Vec3<float> bottomLeftFrontPoint;
	Vec3<float> topRightRearPoint;
	
	Box(Vec3<float> blf,Vec3<float> trr)
	{
		bottomLeftFrontPoint = blf;
		topRightRearPoint = trr;
	}
};

