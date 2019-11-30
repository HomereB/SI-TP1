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
#include<chrono>

const int width = 512;
const int height = 512;

std::random_device rd;
std::mt19937 gen(rd());

int profondeurMax = 3;

int nbSpheres = 101;
std::vector<Sphere> spheres;

std::vector<Light> lights;
int nbLightSource = 3;
int nbLightsPerSource = 15;
int lightIntensity = 500000;
Vec3<float> lightColor = { 1,1,1 };

int nbRayonsRandom = 5;
int randomRayOffset = 30;




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

Vec3<float> DrawRay(int profondeur, Ray ray, Intersection intersect,long* nbRayonsLance)
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

	//Lumières
	std::uniform_real_distribution<> disOffsetLight1(251, 261);
	std::uniform_real_distribution<> disOffsetLightHeight1(567, 577);

	for (int i = 0; i < nbLightsPerSource; i++)
	{
		float x = disOffsetLight1(gen);
		float y = disOffsetLightHeight1(gen);
		float z = disOffsetLight1(gen);
		Vec3<float> lightPos = { x,y,z };
		lights.push_back(Light(lightPos, lightColor, lightIntensity));
	}

	std::uniform_real_distribution<> disOffsetLight2(251, 261);
	std::uniform_real_distribution<> disOffsetLightHeight2(-200, -190);

	for (int i = 0; i < nbLightsPerSource; i++)
	{
		float x = disOffsetLight2(gen);
		float y = disOffsetLightHeight2(gen);
		float z = disOffsetLight2(gen);
		Vec3<float> lightPos = { x,y,z };
		lights.push_back(Light(lightPos, lightColor, lightIntensity));
	}

	std::uniform_real_distribution<> disOffsetLight3(251, 261);
	std::uniform_real_distribution<> disOffsetLightHeight3(567, 577);

	for (int i = 0; i < nbLightsPerSource; i++)
	{
		float x = disOffsetLightHeight3(gen);
		float y = disOffsetLight3(gen);
		float z = disOffsetLight3(gen);
		Vec3<float> lightPos = { x,y,z };
		lights.push_back(Light(lightPos, lightColor, lightIntensity));
	}

	//std::uniform_real_distribution<> disOffsetLight4(251, 261);
	//std::uniform_real_distribution<> disOffsetLightHeight4(-200, -190);

	//for (int i = 0; i < nbLightsPerSource; i++)
	//{
	//	float x = disOffsetLightHeight4(gen);
	//	float y = disOffsetLight4(gen);
	//	float z = disOffsetLight4(gen);
	//	Vec3<float> lightPos = { x,y,z };
	//	lights.push_back(Light(lightPos, lightColor, lightIntensity));
	//}

	//std::uniform_real_distribution<> disOffsetLight5(251, 261);
	//std::uniform_real_distribution<> disOffsetLightHeight5(567, 577);

	//for (int i = 0; i < nbLightsPerSource; i++)
	//{
	//	float x = disOffsetLight5(gen);
	//	float y = disOffsetLight5(gen);
	//	float z = disOffsetLightHeight5(gen);
	//	Vec3<float> lightPos = { x,y,z };
	//	lights.push_back(Light(lightPos, lightColor, lightIntensity));
	//}

	//std::uniform_real_distribution<> disOffsetLight6(251, 261);
	//std::uniform_real_distribution<> disOffsetLightHeight6(-200, -190);

	//for (int i = 0; i < nbLightsPerSource; i++)
	//{
	//	float x = disOffsetLight6(gen);
	//	float y = disOffsetLight6(gen);
	//	float z = disOffsetLightHeight6(gen);
	//	Vec3<float> lightPos = { x,y,z };
	//	lights.push_back(Light(lightPos, lightColor, lightIntensity));
	//}

	//sphères 

	std::uniform_real_distribution<> disPosSphere(10, 500);
	std::uniform_real_distribution<> disHeightSphere(10, 380);
	std::uniform_real_distribution<> disRayonSphere(8, 16);
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
	}

	//sphere mirroir
	
	nbSpheres++; 
	{
		float r = 128;
		float x = 256;
		float y = 512-128;
		float z = 1024;
		Vec3<float> c = { x, y, z };
		Vec3<float> col = {0,0,0};
		float a = 1;
		Sphere s(c, r, col, a);
		spheres.push_back(s);
	}



	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	RGBQUAD colorPixel;
	Vec3<float> color;
	colorPixel.rgbReserved = 255;

	int nbRayonsLanceTotal = 0;
	long nbRayonsLances[width] = { 0 };

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	for (int j = 0; j < height; j++) {
		
		#pragma omp parallel for
		for (int i = 0; i < width; i++) {
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
				color = DrawRay(0, firstRay, firstIntersect,nbRayonsLances+i);
			}
			colorPixel.rgbRed = std::clamp((int)color.x, 0, 255);
			colorPixel.rgbGreen = std::clamp((int)color.y, 0, 255);
			colorPixel.rgbBlue = std::clamp((int)color.z, 0, 255);

			FreeImage_SetPixelColor(bitmap, i, j, &colorPixel);
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	for (int i = 0; i < width;i++)
	{
		nbRayonsLanceTotal += nbRayonsLances[i];
	}
	std::cout << "nombre de rayons lances : "<<nbRayonsLanceTotal<<std::endl;
	std::cout << "temps ecoule : " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

	FreeImage_Save(FIF_PNG, bitmap, "c_bo.png");
	FreeImage_DeInitialise();
}



