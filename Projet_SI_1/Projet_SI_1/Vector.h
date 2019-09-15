#pragma once
#include <iostream>

template<typename T>
struct Vec3 {
	T x, y, z;
};


//Operateur egalitaire
template<typename T>
bool operator==(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
};

//Operateur multiplicateur
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3{ a.x * b.x, a.y * b.y, a.z * b.z };
};

//Operateur diviseur
template<typename T>
Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3{ a.x / b.x, a.y / b.y, a.z / b.z };
};

//Operateur de soustraction
template<typename T>
Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3{ a.x - b.x, a.y - b.y, a.z - b.z };
};

//Operateur d'addition
template<typename T>
Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3{ a.x + b.x, a.y + b.y, a.z + b.z };
};

//Operateur de sortie
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& a) {
	os <<a.x << "," << a.y << "," << a.z << std::endl;
	return os;
};

//Calcul de la nomre d'un vecteur
template<typename T>
float norm(const Vec3<T>& v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
};

//Calcul du produit scalaire
template<typename T>
float dot(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
};

//Calcul du produit vectoriel
template<typename T>
Vec3<T> vect(const Vec3<T>& a, const Vec3<T>& b) {
	
	Vec3<T> res;

	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
	
	return res;
};



