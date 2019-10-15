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
int profondeurMax = 2;
int nbSpheres = 5;
std::vector<Sphere> spheres;
std::vector<Light> lights;
int nbLightSource = 1;
int nbLightsPerSource = 100;
int lightIntensity = 2000000;

Vec3<float> lightColor = { 1,1,1 };
int nbRayonsRandom = 10;

std::optional<float> Intersect(Ray ray,Sphere s)
{
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

Vec3<float> DrawRay(int profondeur, Ray ray, Intersection intersect)
{
	Vec3<float> color = { 0,0,0 };
	if (profondeur < profondeurMax)
	{
		if (spheres[intersect.indexIntersectedItem].albedo == 1) //gestion mirroirs
		{
			Vec3<float> dirReflect = intersect.normale * dot(ray.dir * -1.0f, intersect.normale) * 2.0f + ray.dir * -1.0f;
			normalize(dirReflect);
			Ray newRay(intersect.ptIntersection, dirReflect);

			int indexIntersectedSphere = -1;
			float sphereDistance = 9999999;
			bool gotIntersected = false;

			for (int l = 0; l < nbSpheres; l++)
			{
				std::optional<float> resReflect = Intersect(newRay, spheres[l]);
				if (resReflect.has_value() && resReflect.value() < sphereDistance)
				{
					indexIntersectedSphere = l;
					sphereDistance = resReflect.value();
					gotIntersected = true;
				}
			}
			if (gotIntersected)
			{
				Vec3<float> newPtInter = intersect.ptIntersection + dirReflect * sphereDistance * 0.99f;
				Vec3<float> newNormale = newPtInter - spheres[indexIntersectedSphere].center;
				normalize(newNormale);
				Intersection newIntersect = Intersection(newPtInter, newNormale, indexIntersectedSphere, sphereDistance);
				color = DrawRay(profondeur + 1, newRay, newIntersect);
			}
			else
			{
				return color;
			}
		}

		else //gestion spheres avec lumières
		{
			//std::vector<Vec3<float>> indirectColors;
			//indirectColors.resize(nbRayonsRandom);

			//for (int i = 0; i < nbRayonsRandom; i++) //eclairage indirect
			//{
			//	Vec3<float> dirILight = intersect.normale * dot(ray.dir * -1.0f, intersect.normale) * 2.0f + ray.dir * -1.0f;
			//	normalize(dirILight);
			//	Ray newRay(intersect.ptIntersection, dirILight);
			//}

			//for (int i = 0; i < nbRayonsRandom; i++)
			//{
			//	color = color + indirectColors[i];
			//}

			std::vector<Vec3<float>> directColors;//eclairage direct
			directColors.resize(nbLightsPerSource * nbLightSource);

			for (int i = 0; i < nbLightsPerSource * nbLightSource; i++)
			{
				Vec3<float> dirLight = lights[i].pos - intersect.ptIntersection;
				float lightDistance = norm(dirLight);
				normalize(dirLight);
				Ray newRay(intersect.ptIntersection, dirLight);
				for (int l = 0; l < nbSpheres; l++)
				{
					bool gotIntersected = false;
					std::optional<float> resReflect = Intersect(newRay, spheres[l]);

					if (resReflect.has_value())
					{
						gotIntersected = true;
					}
					if (!gotIntersected)
					{
						float scal = std::abs(dot(dirLight, intersect.normale) / (norm(dirLight) * norm(intersect.normale)));
						color.x += spheres[intersect.indexIntersectedItem].color.x * lights[i].intensity * scal * lights[i].color.x / (nbLightsPerSource * lightDistance * lightDistance);
						color.y += spheres[intersect.indexIntersectedItem].color.y * lights[i].intensity * scal * lights[i].color.y / (nbLightsPerSource * lightDistance * lightDistance);
						color.z += spheres[intersect.indexIntersectedItem].color.z * lights[i].intensity * scal * lights[i].color.z / (nbLightsPerSource * lightDistance * lightDistance);
					}
				}
			}
			for (int i = 0; i < nbLightsPerSource * nbLightSource; i++)
			{
				color = color + directColors[i];
			}
		}
		return color;
	}
	return color;
}

int main()
{

	std::srand((int)std::time(nullptr));
	Vec3<float> colorSphere = { 1,1,1 };

	//box
	float rBoxRear = 100000;
	float xBoxRear = 256;
	float yBoxRear = 256;
	float zBoxRear = 101000;
	Vec3<float> centerBoxRear = { xBoxRear, yBoxRear, zBoxRear };
	Sphere sphereRear(centerBoxRear, rBoxRear, colorSphere,0);
	std::cout << centerBoxRear << rBoxRear << std::endl;
	spheres.push_back(sphereRear);


	float rBoxLeft = 100000;
	float xBoxLeft = -99500;
	float yBoxLeft = 256;
	float zBoxLeft = 10000;
	Vec3<float> centerBoxLeft = { xBoxLeft, yBoxLeft, zBoxLeft };
	Sphere sphereLeft(centerBoxLeft, rBoxLeft, colorSphere - Vec3<float>{1.0f, 0.0f, 0.0f},0);
	std::cout << centerBoxLeft << rBoxLeft << std::endl;
	spheres.push_back(sphereLeft);

	float rBoxRight = 100000;
	float xBoxRight = 100012;
	float yBoxRight = 256;
	float zBoxRight = 10000;
	Vec3<float> centerBoxRight = { xBoxRight, yBoxRight, zBoxRight };
	Sphere sphereRight(centerBoxRight, rBoxRight, colorSphere - Vec3<float>{0.0f, 1.0f, 0.0f},0);
	std::cout << centerBoxRight << rBoxRight << std::endl;
	spheres.push_back(sphereRight);

	float rBoxBottom = 100000;
	float xBoxBottom = 256;
	float yBoxBottom = 100012;
	float zBoxBottom = 10000;
	Vec3<float> centerBoxBottom = { xBoxBottom, yBoxBottom, zBoxBottom };
	Sphere sphereBottom(centerBoxBottom, rBoxBottom, colorSphere - Vec3<float>{0.0f, 0.0f, 1.0f},0);
	std::cout << centerBoxBottom << rBoxBottom << std::endl;
	spheres.push_back(sphereBottom);

	float rBoxTop = 100000;
	float xBoxTop = 256;
	float yBoxTop = -99500;
	float zBoxTop = 10000;
	Vec3<float> centerBoxTop = { xBoxTop, yBoxTop, zBoxTop };
	Sphere sphereTop(centerBoxTop, rBoxTop, colorSphere - Vec3<float>{0.0f, 0.0f, 1.0f},0);
	std::cout << centerBoxTop << rBoxTop << std::endl;
	spheres.push_back(sphereTop);

	for (int i = 0; i < nbLightsPerSource; i++)
	{
		float x = 251 + std::rand() % 10; //->std::uniform_real_distribution
		float y = 477 + std::rand() % 10;
		float z = 251 + std::rand() % 10;
		Vec3<float> lightPos = { x,y,z };
		lights.push_back(Light(lightPos, lightColor, lightIntensity));
	}

	nbSpheres += 4;

	for (int i = 0; i < nbSpheres-5; i++)
	{
		float r = 16 + std::rand() % 48;
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
		float r = 64;
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
	RGBQUAD colorPixel;
	Vec3<float> color;
	colorPixel.rgbReserved = 255;
	#pragma exp parallel for
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			color.x = 0;
			color.y = 0;
			color.z = 0;

			Vec3<float> rDir = { 0.0,0.0,1.0 };
			normalize(rDir);
			Vec3<float> rPos = { i,j,0.0 };
			Ray firstRay = Ray(rPos, rDir);


			int indexIntersectedSphere = -1;
			float sphereDistance = 9999999;
			bool gotIntersected = false;

			for (int l = 0; l < nbSpheres; l++)
			{
				std::optional<float> resReflect = Intersect(firstRay, spheres[l]);
				if (resReflect.has_value() && resReflect.value() < sphereDistance)
				{
					indexIntersectedSphere = l;
					sphereDistance = resReflect.value();
					gotIntersected = true;
				}
			}
			if (gotIntersected)
			{
				Vec3<float> firstPtInter = rPos + rDir * sphereDistance * 0.99f;
				Vec3<float> firstNormale = firstPtInter - spheres[indexIntersectedSphere].center;
				normalize(firstNormale);
				Intersection firstIntersect = Intersection(firstPtInter, firstNormale, indexIntersectedSphere, sphereDistance);
				color = DrawRay(0, firstRay, firstIntersect);
			}
			//if (distIntersectedSphere != -1)
			//{
				



				//for (int i = 0; i < lights.size(); i++)
				//{
				//	bool gotIntersected = false;

				//	Vec3<float> ptInter = ray.pos + ray.dir * distIntersectedSphere;
				//	Vec3<float> normale = { ptInter.x - spheres[intersectedSphere].center.x, ptInter.y - spheres[intersectedSphere].center.y, ptInter.z - spheres[intersectedSphere].center.z };
				//	normalize(normale);
				//	Vec3<float> dirLight = lights[i].pos - ptInter;
				//	ptInter = ptInter + dirLight * (float)0.001;
				//	float lightDistance = norm(dirLight);
				//	normalize(dirLight);

				//	Ray rayToLight(ptInter, dirLight);
				//	for (int l = 0; l < nbSpheres; l++)
				//	{
				//		std::optional<float> resLight = Intersect(rayToLight, spheres[l]);
				//		if (resLight.has_value() && resLight.value()<lightDistance)
				//		{
				//			gotIntersected = true;
				//		}
				//	}
				//	if (!gotIntersected)
				//	{
				//		float scal = std::abs(dot(dirLight, normale) / (norm(dirLight) * norm(normale)));

				//		colorR += spheres[intersectedSphere].color.x * lights[i].intensity * scal * lights[i].color.x / (nbLightsPerSource * lightDistance * lightDistance);
				//		colorG += spheres[intersectedSphere].color.y * lights[i].intensity * scal * lights[i].color.y / (nbLightsPerSource * lightDistance * lightDistance);
				//		colorB += spheres[intersectedSphere].color.z * lights[i].intensity * scal * lights[i].color.z / (nbLightsPerSource * lightDistance * lightDistance);
				//	}
				//	else if (spheres[intersectedSphere].albedo != 0)
				//	{
				//		Vec3<float> dirReflection = normale* dot(rDir*-1.0f,normale) *2.0f+rDir; /*   2 * N * dot(N, -I) + I   */
				//		Ray rayReflect(ptInter, dirReflection);
				//		int indexIntersectedSphere = -1;
				//		float sphereDistance = 9999999;
				//		for (int l = 0; l < nbSpheres; l++)
				//		{
				//			std::optional<float> resReflect = Intersect(rayReflect, spheres[l]);
				//			if (resReflect.has_value() && resReflect.value() < sphereDistance)
				//			{
				//				indexIntersectedSphere = l;
				//				sphereDistance = resReflect.value();
				//				gotIntersected = true;
				//			}
				//		}
				//		if (!gotIntersected)
				//		{
				//			float scal = std::abs(dot(dirReflection, normale) / (norm(dirReflection) * norm(normale)));

				//			colorR += spheres[intersectedSphere].albedo * lights[i].intensity * scal * spheres[indexIntersectedSphere].color.x / (nbLightsPerSource * lightDistance * lightDistance);
				//			colorG += spheres[intersectedSphere].albedo * lights[i].intensity * scal * lights[indexIntersectedSphere].color.y / (nbLightsPerSource * lightDistance * lightDistance);
				//			colorB += spheres[intersectedSphere].albedo * lights[i].intensity * scal * lights[indexIntersectedSphere].color.z / (nbLightsPerSource * lightDistance * lightDistance);
				//		}
				//		else
				//		{
				//			colorR += 0;
				//			colorG += 0;
				//			colorB += 0;
				//		}
				//	}
				//	else
				//	{
				//		colorR += 0;
				//		colorG += 0;
				//		colorB += 0;
				//	}
				//}
			//}
			colorPixel.rgbRed = std::clamp((int)color.x, 0, 255);
			colorPixel.rgbGreen = std::clamp((int)color.y, 0, 255);
			colorPixel.rgbBlue = std::clamp((int)color.z, 0, 255);

			FreeImage_SetPixelColor(bitmap, i, j, &colorPixel);
		}
	}

	FreeImage_Save(FIF_PNG, bitmap, "c_bo.png");

	FreeImage_DeInitialise();
}



