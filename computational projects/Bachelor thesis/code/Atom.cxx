#include "Atom.h"
#include <iostream>

using namespace std;

Atom::Atom(double x, double y, double z, int name):
Vector3D(x,y,z),
i_name(name)
{	
}

Atom::Atom(Vector3D coord, int name):
Vector3D(coord),  //implementato di default nelle classi
i_name(name)
{
}

Atom::~Atom()  //per ora vuoto
{
}


void Atom::Show_myself() const   //sostituirlo chiamando la func di Vector
{
	cout<<"Atom number " << i_name <<":" << endl;
	Vector3D::Show_myself();  

	return;
}

void Atom::Write_myself(std::ostream& os) const
{
	os <<"ATOM";
	os.width(7);
	os << i_name+1 <<"  CA  GLY A";
	os.width(4);
	os << i_name+1;
	Vector3D::Write_coordinates(os);  //scrive le coordinate del vettore
	os.width(6);
	os << "0.50";
	os.width(6);
	os << "0.50";
	os.width(4);
	os << i_name+1 << endl;
}
