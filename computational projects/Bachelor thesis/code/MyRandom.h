#ifndef MYRANDOM_H
#define MYRANDOM_H

#include "TRandom3.h"
#include "Chain.h"
#include <string>

class MyRandom: public TRandom3{

public:
	MyRandom(int seed, double probAdd, int min_elements, int max_elements, double radius, double dist_ex);
	virtual ~MyRandom();

	double Random_theta();
	double Random_thetaVex();
	double Random_phi();
	string Choose_action();
	void Update_state(string s_action_done, int i_name); //passo ciò che ho fatto e il nome del Ca add/rmv

private:
	//informazioni sulla costruzione
	const double d_probAdd;   //prob di aggiungere (test permettendo)
	const double d_probSx;	  //prob lavorare a sx
	const int i_min_elements;
	const int i_max_elements;
	double d_K;  //parametro per campionare secondo il volume escluso 
	//informazioni sullo stato attuale
	int i_elements;
	bool b_last;
	bool b_first;
	bool b_lmin;
	bool b_lmax;

	ClassDef(MyRandom,1);
};

#endif

