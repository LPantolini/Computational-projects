#include "Vector3D.h"
#include <iostream>

Vector3D::Vector3D():
d_x(0.0),
d_y(0.0),
d_z(0.0)
{
}

Vector3D::Vector3D(double x, double y, double z):
d_x(x),
d_y(y),
d_z(z)
{
}

Vector3D::~Vector3D()
{
}

double Vector3D::Squared_distance(const Vector3D& V) const
{
	double dx = (d_x - V.d_x);
	double dy = (d_y - V.d_y);
	double dz = (d_z - V.d_z);

	return (dx*dx + dy*dy + dz*dz);  //distance ^2
}

double Vector3D::Norm() const
{
	return sqrt(d_x*d_x + d_y*d_y + d_z*d_z);
}

void Vector3D::Write_coordinates(std::ostream& os) const
{
	os.width(12);
	os << d_x;
	os.width(8);  
	os << d_y;
	os.width(8);  
	os << d_z;
}

void Vector3D::Show_myself() const
{
	std::cout <<"(" << d_x << std::endl;   
	std::cout << d_y << std::endl;
	std::cout << d_z <<")" << std::endl;

	return;
}

Vector3D Vector3D::operator+(const Vector3D& V) const
{
	return Vector3D( (d_x + V.d_x), (d_y + V.d_y), (d_z + V.d_z) );
}

Vector3D Vector3D::operator-(const Vector3D& V) const
{
	return Vector3D( (d_x - V.d_x), (d_y - V.d_y), (d_z - V.d_z) );
}

Vector3D Vector3D::operator^(const Vector3D& V) const
{
	return Vector3D( (d_y*V.d_z - d_z*V.d_y), (d_z*V.d_x - d_x*V.d_z), (d_x*V.d_y - d_y*V.d_x) );
}

Vector3D Vector3D::operator*(double K) const
{
	return Vector3D(d_x*K, d_y*K, d_z*K);
}


