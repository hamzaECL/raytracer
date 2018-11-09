#pragma once
#include"math.h"
#include<iostream>
using namespace std;

class Vector
{
public:
	float x, y, z;
	Vector(float xx = 0, float yy = 0, float zz = 0);
	
	Vector operator-();
	Vector operator-(Vector h);
	Vector operator+(Vector h);
	Vector operator*(float h);
	Vector operator=(Vector h);
	float dot(Vector h);
	Vector prodVect(Vector h);
	float norm();
	Vector operator/(float h);
	float normsquare();
};

Vector operator*(float h, Vector V);
Vector normalize(Vector h);
