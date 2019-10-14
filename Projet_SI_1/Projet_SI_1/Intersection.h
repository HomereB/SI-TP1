#pragma once
#include "Vector.h"

class Intersection
{
public:
	Vec3<float> ptIntersection;
	Vec3<float> normale;
	int indexIntersectedItem;
	float distIntersection;

	Intersection(Vec3<float> ptInter, Vec3<float> norm, int indexInter, float dist)
	{
		ptIntersection = ptInter;
		normale = norm;
		indexIntersectedItem = indexInter;
		distIntersection = dist;
	}
};

