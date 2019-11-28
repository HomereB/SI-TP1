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
#include "Box.h"
#include <random>

const int width = 512;
const int height = 512;

std::random_device rd;
std::mt19937 gen(rd());

int profondeurMax = 2;

int nbSpheres = 25;
std::vector<Sphere> spheres;

std::vector<Light> lights;
int nbLightSource = 1;
int nbLightsPerSource = 100;
int lightIntensity = 300000;
Vec3<float> lightColor = { 1,1,1 };

int nbRayonsRandom = 25;
int randomRayOffset = 15;




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

Vec3<float> DrawRay(int profondeur, Ray ray, Intersection intersect,int* nbRayonsLance)
{
	(*nbRayonsLance)++;
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
				color = color + DrawRay(profondeur + 1, newRay, newIntersect, nbRayonsLance);
			}

			else
			{
				return color;
			}
		}

		else //gestion spheres avec lumières
		{
			Vec3<float> indirectColors = { 0,0,0 };

			for (int i = 0; i < nbRayonsRandom; i++) //eclairage indirect
			{
				std::uniform_real_distribution<> disRayOffset(-randomRayOffset, randomRayOffset);
				Vec3<float> dirILight = { intersect.normale.x - disRayOffset(gen) , intersect.normale.y - disRayOffset(gen), intersect.normale.z - disRayOffset(gen) }; // rayon random autour de la normale
				normalize(dirILight);
				Ray newRay(intersect.ptIntersection, dirILight);


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
					Vec3<float> newPtInter = intersect.ptIntersection + dirILight * sphereDistance * 0.95f;
					Vec3<float> newNormale = newPtInter - spheres[indexIntersectedSphere].center;
					normalize(newNormale);
					Intersection newIntersect = Intersection(newPtInter, newNormale, indexIntersectedSphere, sphereDistance);
					indirectColors =indirectColors + DrawRay(profondeur + 1, newRay, newIntersect, nbRayonsLance)*0.1f/(float)nbRayonsRandom;
				}
			}

			Vec3<float> directColors = { 0,0,0 };//eclairage direct

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

					if (gotIntersected == false)
					{
						float scal = dot(dirLight, intersect.normale) / (norm(dirLight) * norm(intersect.normale));
						if (scal < 0)
						{
							scal = 0;
						}

						directColors.x += spheres[intersect.indexIntersectedItem].color.x * lights[i].intensity * scal * lights[i].color.x / (nbLightsPerSource * lightDistance * lightDistance);
						directColors.y += spheres[intersect.indexIntersectedItem].color.y * lights[i].intensity * scal * lights[i].color.y / (nbLightsPerSource * lightDistance * lightDistance);
						directColors.z += spheres[intersect.indexIntersectedItem].color.z * lights[i].intensity * scal * lights[i].color.z / (nbLightsPerSource * lightDistance * lightDistance);
					}
				}
			}
				color = color + directColors;
		}
		return color;
	}
	return color;
}

int main()
{
	Vec3<float> colorSphere = { 1,1,1 };
	Vec3<float> pointCamera = {256,256,-1200};

	////box
	//float rBoxRear = 100000;
	//float xBoxRear = 256;
	//float yBoxRear = 256;
	//float zBoxRear = 101000;
	//Vec3<float> centerBoxRear = { xBoxRear, yBoxRear, zBoxRear };
	//Sphere sphereRear(centerBoxRear, rBoxRear, colorSphere,0);
	//std::cout << centerBoxRear << rBoxRear << std::endl;
	//spheres.push_back(sphereRear);


	//float rBoxLeft = 100000;
	//float xBoxLeft = -99500;
	//float yBoxLeft = 256;
	//float zBoxLeft = 10000;
	//Vec3<float> centerBoxLeft = { xBoxLeft, yBoxLeft, zBoxLeft };
	//Sphere sphereLeft(centerBoxLeft, rBoxLeft, colorSphere - Vec3<float>{1.0f, 0.0f, 0.0f},0);
	//std::cout << centerBoxLeft << rBoxLeft << std::endl;
	//spheres.push_back(sphereLeft);

	//float rBoxRight = 100000;
	//float xBoxRight = 100012;
	//float yBoxRight = 256;
	//float zBoxRight = 10000;
	//Vec3<float> centerBoxRight = { xBoxRight, yBoxRight, zBoxRight };
	//Sphere sphereRight(centerBoxRight, rBoxRight, colorSphere - Vec3<float>{0.0f, 1.0f, 0.0f},0);
	//std::cout << centerBoxRight << rBoxRight << std::endl;
	//spheres.push_back(sphereRight);

	//float rBoxBottom = 100000;
	//float xBoxBottom = 256;
	//float yBoxBottom = 100012;
	//float zBoxBottom = 10000;
	//Vec3<float> centerBoxBottom = { xBoxBottom, yBoxBottom, zBoxBottom };
	//Sphere sphereBottom(centerBoxBottom, rBoxBottom, colorSphere - Vec3<float>{0.0f, 0.0f, 1.0f},0);
	//std::cout << centerBoxBottom << rBoxBottom << std::endl;
	//spheres.push_back(sphereBottom);

	//float rBoxTop = 100000;
	//float xBoxTop = 256;
	//float yBoxTop = -99500;
	//float zBoxTop = 10000;
	//Vec3<float> centerBoxTop = { xBoxTop, yBoxTop, zBoxTop };
	//Sphere sphereTop(centerBoxTop, rBoxTop, colorSphere - Vec3<float>{0.0f, 0.0f, 1.0f},0);
	//std::cout << centerBoxTop << rBoxTop << std::endl;
	//spheres.push_back(sphereTop);

	std::uniform_real_distribution<> disOffsetLight(251, 261);
	std::uniform_real_distribution<> disOffsetLightHeight(467, 477);

	for (int i = 0; i < nbLightsPerSource; i++)
	{
		float x = disOffsetLight(gen);
		float y = disOffsetLightHeight(gen);
		float z = disOffsetLight(gen);
		Vec3<float> lightPos = { x,y,z };
		lights.push_back(Light(lightPos, lightColor, lightIntensity));
	}

	//nbSpheres += 4;

	std::uniform_real_distribution<> disPosSphere(64, 448);
	std::uniform_real_distribution<> disHeightSphere(64, 320);
	std::uniform_real_distribution<> disRayonSphere(16, 64);
	std::uniform_real_distribution<> disColorSphere(0, 1);

	for (int i = 0; i < nbSpheres; i++)
	{
		float r = disRayonSphere(gen);
		float x = disPosSphere(gen);
		float y = disHeightSphere(gen);
		float z = disPosSphere(gen);
		Vec3<float> c = { x, y, z };
		Vec3<float> col = colorSphere - Vec3<float>{(float)disColorSphere(gen), (float)disColorSphere(gen), (float)disColorSphere(gen)};
		float a = 0;
		Sphere s(c, r,col,a);
		spheres.push_back(s);
		std::cout << c << r << std::endl;
	}
	nbSpheres++; //sphere mirroir
	{
		float r = 64;
		float x = 256;
		float y = 512-128;
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

	int nbRayonsLanceTotal = 0;
	for (int j = 0; j < height; j++) {
		
		#pragma omp parallel
		for (int i = 0; i < width; i++) {
			int nbRayonsLanceInter = 0;
			color.x = 0;
			color.y = 0;
			color.z = 0;

			Vec3<float> rDir = { i,j,0.0f };
			rDir =rDir - pointCamera;
			normalize(rDir);
			Vec3<float> rPos = { i, j, 0 };
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
				color = DrawRay(0, firstRay, firstIntersect,&nbRayonsLanceInter);
			}
			colorPixel.rgbRed = std::clamp((int)color.x, 0, 255);
			colorPixel.rgbGreen = std::clamp((int)color.y, 0, 255);
			colorPixel.rgbBlue = std::clamp((int)color.z, 0, 255);

			FreeImage_SetPixelColor(bitmap, i, j, &colorPixel);
			nbRayonsLanceTotal += nbRayonsLanceInter;
		}
	}
	std::cout << "nombre de rayons lances : "<<nbRayonsLanceTotal<<std::endl;
	FreeImage_Save(FIF_PNG, bitmap, "c_bo.png");
	FreeImage_DeInitialise();
}



