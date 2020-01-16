#ifndef FOLDER_H
#define FOLDER_H

#include "MyRandom.h"
#include "Atom.h"
#include "Chain.h"

class Folder{

public:

	Folder(double T, MyRandom* random, Chain* chain, Chain* native, bool Vex, bool Metropolis, bool debug);
	~Folder();

	void Set_action();
	bool Test_action();  
	void Perform_action();
	Atom* Craft_Atom();

private:
	
	string s_action;
	
	MyRandom* pnt_random;
	Chain* pnt_mychain;
	Chain* pnt_native;
	//parametri che definiscono i test
	const double d_T;    	//temperatura del sistema
	const bool b_Vex;		//costruisco il test a seconda dei controlli che voglio fare
	const bool b_Metropolis;
	const bool b_debug;
	//mossa da testare
	Atom* pnt_new_Ca; 
	int i_Ca_name;
	//immagazzina contatti per Ca aggiunti
	vector<int> v_contacts;

};

#endif