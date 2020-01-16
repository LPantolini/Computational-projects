#ifndef VECTOR3D_H
#define VECTOR3D_H

class Vector3D{

public:
	Vector3D();
	Vector3D(double x, double y, double z);
	~Vector3D();

	double Squared_distance(const Vector3D& V) const;
	double Norm() const;
	void Write_coordinates(std::ostream& os) const;
	void Show_myself() const;
	Vector3D operator+(const Vector3D& V) const;
	Vector3D operator-(const Vector3D& V) const;
	Vector3D operator^(const Vector3D& V) const;  //prodotto vettore
	Vector3D operator*(double K) const; 		  //prodotto scalare

private:
	double d_x;
	double d_y;
	double d_z;
};

#endif