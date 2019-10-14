// Projet_SI_1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <FreeImage.h>
#include <conio.h>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include "Intersection.h"
#include <optional>
#include <vector>
#include <ctime>
#include "Light.h"
#include <algorithm>
#include <math.h>
//#pragma exp parallel for

const int width = 512;
const int height = 512;
int profondeurMax = 5;
int nbSpheres = 5;
std::vector<Sphere> spheres;
std::vector<Light> lights;
int nbLights = 100;
Vec3<float> lightColor = { 1,1,1 };

std::optional<float> Intersect(Ray ray,Sphere s)
{
	Vec3<float> res;

	float a = 1;
	float b = 2 * (dot(ray.pos, ray.dir)-dot(s.center,ray.dir));
	float c = dot(ray.pos, ray.pos) + dot(s.center, s.center) - 2 * dot(s.center, ray.pos) - s.ray*s.ray;

	float delta = (b * b) - (4 * a * c);
	if (delta == 0)
	{
		return -b / (2 * a);
	}


	else if (delta > 0)
	{
		float r1 = (-b - std::sqrt(delta)) / (2 * a);
		float r2 = (-b + std::sqrt(delta)) / (2 * a);

		if(r1<0 && r2<0)
			return std::nullopt;

		else if (r1 < 0 && r2 >= 0)
			return r2;

		else 
			return r1;
	}


	else if (delta < 0)
	{
		return std::nullopt;
	}
}

Vec3<float> DrawRay(int profondeur,Ray ray,Intersection intersect)
{
	Vec3<float> color = { 0,0,0 }; //check albedo sphere initiale -> intersection sphere avec rayon et resultat adequat-> nouvelle intersection et lancer du nouveau rayon

	int indexIntersectedSphere = -1;
	float sphereDistance = 9999999;
	bool gotIntersected = false;

	for (int l = 0; l < nbSpheres; l++)
	{
		std::optional<float> resReflect = Intersect(ray, spheres[l]);
		if (resReflect.has_value() && resReflect.value() < sphereDistance)
		{		
			indexIntersectedSphere = l;
			sphereDistance = resReflect.value();
			gotIntersected = true;
		}
	}

	if (gotIntersected)
	{
		Vec3<float> newPtIntersect = intersect.ptIntersection + ray.dir * sphereDistance;
		Vec3<float> normaleIntersect = newPtIntersect - spheres[indexIntersectedSphere].center;
		normalize(normaleIntersect);
		Intersection inter = Intersection(intersect.ptIntersection + ray.dir * sphereDistance, , indexIntersectedSphere, sphereDistance);
		if (spheres[intersect.indexIntersectedItem].albedo == 1)
		{
			Vec3<float> dirReflection = intersect.normale * dot(ray.dir * -1.0f, intersect.normale) * 2.0f + ray.dir * -1.0f; /*   2 * N * dot(N, -I) + I   */
			Ray rayReflect(intersect.ptIntersection, dirReflection);
			DrawRay(profondeur++, rayReflect, inter);
		}
		else
		{
			Vec3<float> dirReflection = intersect.normale * dot(ray.dir * -1.0f, intersect.normale) * 2.0f + ray.dir * -1.0f; /*   2 * N * dot(N, -I) + I   */
			Ray rayReflect(intersect.ptIntersection, dirReflection);
			DrawRay(profondeur++, rayReflect, inter);
		}
	}
	return color;
}

int main()
{

	std::srand(std::time(nullptr));
	Vec3<float> colorSphere = { 1,1,1 };

	int intensity = 20000000;

	//box

	int rBoxRear = 100000;
	int xBoxRear = 256;
	int yBoxRear = 256;
	int zBoxRear = 101000;
	Vec3<float> centerBoxRear = { xBoxRear, yBoxRear, zBoxRear };
	Sphere sphereRear(centerBoxRear, rBoxRear, colorSphere,0);
	std::cout << centerBoxRear << rBoxRear << std::endl;
	spheres.push_back(sphereRear);


	int rBoxLeft = 100000;
	int xBoxLeft = -99500;
	int yBoxLeft = 256;
	int zBoxLeft = 10000;
	Vec3<float> centerBoxLeft = { xBoxLeft, yBoxLeft, zBoxLeft };
	Sphere sphereLeft(centerBoxLeft, rBoxLeft, colorSphere - Vec3<float>{1.0f, 0.0f, 0.0f},0);
	std::cout << centerBoxLeft << rBoxLeft << std::endl;
	spheres.push_back(sphereLeft);

	int rBoxRight = 100000;
	int xBoxRight = 100012;
	int yBoxRight = 256;
	int zBoxRight = 10000;
	Vec3<float> centerBoxRight = { xBoxRight, yBoxRight, zBoxRight };
	Sphere sphereRight(centerBoxRight, rBoxRight, colorSphere - Vec3<float>{0.0f, 1.0f, 0.0f},0);
	std::cout << centerBoxRight << rBoxRight << std::endl;
	spheres.push_back(sphereRight);

	int rBoxBottom = 100000;
	int xBoxBottom = 256;
	int yBoxBottom = 100012;
	int zBoxBottom = 10000;
	Vec3<float> centerBoxBottom = { xBoxBottom, yBoxBottom, zBoxBottom };
	Sphere sphereBottom(centerBoxBottom, rBoxBottom, colorSphere - Vec3<float>{0.0f, 0.0f, 1.0f},0);
	std::cout << centerBoxBottom << rBoxBottom << std::endl;
	spheres.push_back(sphereBottom);

	int rBoxTop = 100000;
	int xBoxTop = 256;
	int yBoxTop = -99500;
	int zBoxTop = 10000;
	Vec3<float> centerBoxTop = { xBoxTop, yBoxTop, zBoxTop };
	Sphere sphereTop(centerBoxTop, rBoxTop, colorSphere - Vec3<float>{0.0f, 0.0f, 1.0f},0);
	std::cout << centerBoxTop << rBoxTop << std::endl;
	spheres.push_back(sphereTop);

	for (int i = 0; i < nbLights; i++)
	{
		float x = 251 + std::rand() % 10; //->std::uniform_real_distribution
		float y = 477 + std::rand() % 10;
		float z = 251 + std::rand() % 10;
		Vec3<float> lightPos = { x,y,z };
		lights.push_back(Light(lightPos, lightColor, intensity));
	}

	nbSpheres += 4;

	for (int i = 0; i < nbSpheres-5; i++)
	{
		int r = 16 + std::rand() % 48;
		float x = 64 + std::rand() % 384;
		float y = 64 + std::rand() % 256;
		float z = 64 + std::rand() % 384;
		Vec3<float> c = { x, y, z };
		Vec3<float> col = colorSphere - Vec3<float>{(std::rand() % 100) / 100.f, (std::rand() % 100) / 100.f, (std::rand() % 100) / 100.f};
		float a = 0;
		Sphere s(c, r,col,a);
		spheres.push_back(s);
		std::cout << c << r << std::endl;
	}
	nbSpheres++; //sphere mirroir
	{
		int r = 64;
		float x = 256;
		float y = 256;
		float z = 512;
		Vec3<float> c = { x, y, z };
		Vec3<float> col = {0,0,0};
		float a = 1;
		Sphere s(c, r, col, a);
		spheres.push_back(s);
		std::cout << c << r << std::endl;
	}

	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	RGBQUAD color;
	color.rgbRed = 0;
	color.rgbGreen = 0;
	color.rgbBlue = 0;
	color.rgbReserved = 255;
	#pragma exp parallel
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			color.rgbRed = 0;
			color.rgbGreen = 0;
			color.rgbBlue = 0;
			float colorR = 0;
			float colorG = 0;
			float colorB = 0;

			Vec3<float> rDir = { 0.0,0.0,1.0 };
			normalize(rDir);
			Vec3<float> rPos = { i,j,0.0 };
			Ray ray = Ray(rPos, rDir);

			int intersectedSphere = -1;
			float distIntersectedSphere = -1;
			for (int k = 0; k < nbSpheres; k++)
			{
				std::optional<float> res = Intersect(ray, spheres[k]);
				if (res.has_value())
				{
					if (distIntersectedSphere == -1 || distIntersectedSphere > res.value())
					{
						distIntersectedSphere = res.value();
						intersectedSphere = k;
					}
				}
			}
			if (distIntersectedSphere != -1)
			{
				for (int i = 0; i < lights.size(); i++)
				{
					bool gotIntersected = false;

					Vec3<float> ptInter = ray.pos + ray.dir * distIntersectedSphere;
					Vec3<float> normale = { ptInter.x - spheres[intersectedSphere].center.x, ptInter.y - spheres[intersectedSphere].center.y, ptInter.z - spheres[intersectedSphere].center.z };
					normalize(normale);
					Vec3<float> dirLight = lights[i].pos - ptInter;
					ptInter = ptInter + dirLight * (float)0.001;
					float lightDistance = norm(dirLight);
					normalize(dirLight);

					Ray rayToLight(ptInter, dirLight);
					for (int l = 0; l < nbSpheres; l++)
					{
						std::optional<float> resLight = Intersect(rayToLight, spheres[l]);
						if (resLight.has_value() && resLight.value()<lightDistance)
						{
							gotIntersected = true;
						}
					}
					if (!gotIntersected)
					{
						float scal = std::abs(dot(dirLight, normale) / (norm(dirLight) * norm(normale)));

						colorR += spheres[intersectedSphere].color.x * lights[i].intensity * scal * lights[i].color.x / (nbLights * lightDistance * lightDistance);
						colorG += spheres[intersectedSphere].color.y * lights[i].intensity * scal * lights[i].color.y / (nbLights * lightDistance * lightDistance);
						colorB += spheres[intersectedSphere].color.z * lights[i].intensity * scal * lights[i].color.z / (nbLights * lightDistance * lightDistance);
					}
					else if (spheres[intersectedSphere].albedo != 0)
					{
						Vec3<float> dirReflection = normale* dot(rDir*-1.0f,normale) *2.0f+rDir; /*   2 * N * dot(N, -I) + I   */
						Ray rayReflect(ptInter, dirReflection);
						int indexIntersectedSphere = -1;
						float sphereDistance = 9999999;
						for (int l = 0; l < nbSpheres; l++)
						{
							std::optional<float> resReflect = Intersect(rayReflect, spheres[l]);
							if (resReflect.has_value() && resReflect.value() < sphereDistance)
							{
								indexIntersectedSphere = l;
								sphereDistance = resReflect.value();
								gotIntersected = true;
							}
						}
						if (!gotIntersected)
						{
							float scal = std::abs(dot(dirReflection, normale) / (norm(dirReflection) * norm(normale)));

							colorR += spheres[intersectedSphere].albedo * lights[i].intensity * scal * spheres[indexIntersectedSphere].color.x / (nbLights * lightDistance * lightDistance);
							colorG += spheres[intersectedSphere].albedo * lights[i].intensity * scal * lights[indexIntersectedSphere].color.y / (nbLights * lightDistance * lightDistance);
							colorB += spheres[intersectedSphere].albedo * lights[i].intensity * scal * lights[indexIntersectedSphere].color.z / (nbLights * lightDistance * lightDistance);
						}
						else
						{
							colorR += 0;
							colorG += 0;
							colorB += 0;
						}
					}
					else
					{
						colorR += 0;
						colorG += 0;
						colorB += 0;
					}
				}
			}
			color.rgbRed = std::clamp((int)colorR, 0, 255);
			color.rgbGreen = std::clamp((int)colorG, 0, 255);
			color.rgbBlue = std::clamp((int)colorB, 0, 255);

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}

	FreeImage_Save(FIF_PNG, bitmap, "c_bo.png");

	FreeImage_DeInitialise();
}



