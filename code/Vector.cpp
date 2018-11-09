#include "Vector.h"



Vector::Vector(float xx, float yy, float zz)
{
	x = xx;
	y = yy;
	z = zz;
}
Vector Vector::operator-()
{
	return Vector(-x, -y, -z);
}
Vector Vector::operator-(Vector h)
{
	return Vector(x - (h.x), y - h.y, z - h.z);
}
Vector Vector::operator+(Vector h)
{
	return Vector(x + (h.x), y + (h.y), z + (h.z));
}
Vector Vector::operator*(float h)
{
	return Vector(h*x, h*y, h*z);
}
Vector Vector::operator=(Vector h)
{
	x = h.x; y = h.y; z = h.z;
	return *this;
}
float Vector::dot(Vector h)
{
	return x*h.x + y*h.y + z * h.z;
}

Vector Vector::prodVect(Vector h)
{
	return Vector(y*h.z - z*h.y, z*h.x - x*h.z, x*h.y - y*h.x);
}
float Vector::norm()
{
	return sqrt(x*x + y*y + z*z);
}
Vector Vector::operator/(float h)
{
	return Vector(x / h, y / h, z / h);
}
float Vector::normsquare()
{
	return x*x + y*y + z*z;
}

Vector operator*(float h, Vector V)
{
	return Vector(h*V.x, h*V.y, h*V.z);
}

Vector normalize(Vector h)
{
	return h / h.norm();
}

