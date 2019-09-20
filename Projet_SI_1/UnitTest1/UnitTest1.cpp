#include "pch.h"
#include "CppUnitTest.h"
#include "../Projet_SI_1/Ray.h"
#include "../Projet_SI_1/Sphere.h"
#include "../Projet_SI_1/Vector.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Vec3<float> rDir = { 1.0,0.0,0.0 };
			rDir.x = rDir.x / norm(rDir);
			rDir.y = rDir.y / norm(rDir);
			rDir.z = rDir.z / norm(rDir);
			Vec3<float> rPos = { 0.0,0.0,0.0 };

			Vec3<float> sCenter = { 4.0,3.0,0.0 };
			float sRay = 3;

			Ray ray = Ray(rPos, rDir);
			Ray ray = Ray(rPos, rDir);
			Sphere sphere = Sphere(sCenter, sRay);


		}
	};
}
