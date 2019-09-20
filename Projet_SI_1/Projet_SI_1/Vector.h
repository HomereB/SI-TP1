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

//Operateur multiplicateur de vecteur
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x * b.x, a.y * b.y, a.z * b.z };
};

//Operateur diviseur de vecteur
template<typename T>
Vec3<T> operator/(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x / b.x, a.y / b.y, a.z / b.z };
};

//Operateur de soustraction de vecteur
template<typename T>
Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x - b.x, a.y - b.y, a.z - b.z };
};

//Operateur d'addition de vecteur
template<typename T>
Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>{ a.x + b.x, a.y + b.y, a.z + b.z };
};

//Operateur multiplicateur par scalaire
template<typename T>
Vec3<T> operator*(const Vec3<T>& a, T k) {
	return Vec3<T>{ a.x * k, a.y * k, a.z * k };
};

//Operateur diviseur par scalaire
template<typename T>
Vec3<T> operator/(const Vec3<T>& a, T k) {
	return Vec3<T>{ a.x / k, a.y / k, a.z / k };
};

//Operateur de soustraction par scalaire
template<typename T>
Vec3<T> operator-(const Vec3<T>& a, T k) {
	return Vec3<T>{ a.x -k, a.y - k, a.z - k };
};

//Operateur d'addition par scalaire
template<typename T>
Vec3<T> operator+(const Vec3<T>& a, T k) {
	return Vec3<T>{ a.x + k, a.y +k, a.z + k };
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

//Normalisation d'un vecteur
template<typename T>
void normalize( Vec3<T>& v) {
	float n = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	v.x = v.x / n;
	v.y = v.y / n;
	v.z = v.z / n;
};

//Calcul de la nomre carré d'un vecteur
template<typename T>
float normsqr(const Vec3<T>& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
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



