// Projet_SI_1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <FreeImage.h>
#include <conio.h>
#include "Vector.h"

using namespace std;

const int width = 1024;
const int height = 1024;

int main()
{
	Vec3<float> v1 = { 0.0,3.0,0.0 };
	Vec3<float> v2 = { 3.0,0.0,0.0 };

	cout << v1<<v2;
	cout << vect(v1, v2);


	FreeImage_Initialise();
	//cout << FreeImage_GetCopyrightMessage();

	//FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32);
	//RGBQUAD color;

	//for (int j = 0; j < height; j++) {
	//	for (int i = 0; i < width; i++) {
	//		color.rgbBlue = 0;
	//		color.rgbGreen = (double)i / width * 255.0;
	//		color.rgbBlue = (double)j / height * 255.0;
	//		color.rgbReserved = 255;
	//		FreeImage_SetPixelColor(bitmap, i, j, &color);
	//	}
	//}
	//FreeImage_Save(FIF_PNG, bitmap, "c_bo.png");



	_getch();
	FreeImage_DeInitialise();
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
