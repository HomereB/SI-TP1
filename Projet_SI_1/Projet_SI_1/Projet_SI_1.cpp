// Projet_SI_1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <FreeImage.h>
#include <conio.h>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"
#include <optional>
#include <vector>
#include <ctime>


const int width = 512;
const int height = 512;


std::optional<float> intersect(Ray ray,Sphere s)
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

int main()
{

	std::srand(std::time(nullptr));

	Vec3<float> lightPos1 = {256,512,256};
	RGBQUAD lightColor1;
	lightColor1.rgbRed = 200;
	lightColor1.rgbGreen = 200;
	lightColor1.rgbBlue = 200;
	lightColor1.rgbReserved = 255;
	int lightIntensity = 400000;

	int nbSpheres = 4;
	std::vector<Sphere> spheres;
	for (int i = 0; i < nbSpheres; i++)
	{
		int r = 16+std::rand()%48;
		float x = 64+std::rand() % 384;
		float y = 64+std::rand() % 384;
		float z = 64+std::rand() % 384;
		Vec3<float> c = { x, y, z };
		Sphere s(c, r);
		spheres.push_back(s);
		std::cout << c  <<r<< std::endl;
	}
	


	FreeImage_Initialise();

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	RGBQUAD color;
	color.rgbRed = 0;
	color.rgbGreen = 0;
	color.rgbBlue = 0;
	color.rgbReserved = 255;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			color.rgbRed = 0;
			color.rgbGreen = 0;
			color.rgbBlue = 0;

			Vec3<float> rDir = { 0.0,0.0,1.0 };
			normalize(rDir);
			Vec3<float> rPos = { i,j,0.0 };
			Ray ray = Ray(rPos, rDir);

			for (int k = 0; k < nbSpheres; k++)
			{
				std::optional<float> res = intersect(ray, spheres[k]);
				if (res.has_value())
				{
					bool gotIntersected = false;

					Vec3<float> ptInter = ray.pos + ray.dir * res.value();
					Vec3<float> normale = { ptInter.x - spheres[k].center.x, ptInter.y - spheres[k].center.y, ptInter.z - spheres[k].center.z };
					normalize(normale);
					Vec3<float> dirLight = lightPos1 - ptInter;
					ptInter = ptInter +  dirLight * (float)0.0001;
					float lightDistance = norm(dirLight);
					normalize(dirLight);
					
					Ray rayToLight(ptInter, dirLight);
					for (int l = 0; l < nbSpheres; l++)
					{
						std::optional<float> resLight = intersect(rayToLight, spheres[l]);
						if (resLight.has_value())
						{
							gotIntersected = true;
						}
					}
					if (!gotIntersected)
					{
						float scal = dot(dirLight, normale) / (norm(dirLight) * norm(normale) );


						color.rgbRed =     scal * lightColor1.rgbRed ;
						color.rgbGreen =   scal *  lightColor1.rgbGreen ;
						color.rgbBlue =  scal*lightColor1.rgbBlue ;
					}
					else
					{
						color.rgbRed += 0;
						color.rgbGreen += 0;
						color.rgbBlue += 0;
						color.rgbReserved = 255;
					}
				}
			}

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	FreeImage_Save(FIF_PNG, bitmap, "c_bo.png");



	FreeImage_DeInitialise();
}


