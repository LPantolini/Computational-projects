#ifndef ATOM_H
#define ATOM_H

#include "Vector3D.h"
#include <fstream>

class Atom: public Vector3D{

public:
	Atom(double x, double y, double z, int name);
	Atom(Vector3D coord, int name);
	~Atom();

	void Show_myself() const;
	void Write_myself(std::ostream& os) const;
	int Get_name() const {return i_name;}; 
	

private:
	const int i_name; 
	//double d_theta;
	//double d_phi;
};

#endif
