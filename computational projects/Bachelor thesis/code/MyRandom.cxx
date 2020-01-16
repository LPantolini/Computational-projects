#include "MyRandom.h"
#include <iostream>
#include <cmath>

ClassImp(MyRandom)

MyRandom::MyRandom(int seed, double probAdd, int min_elements, int max_elements, double radius, double dist_ex):
TRandom3(seed),
d_probAdd(probAdd),
d_probSx(0.5),
i_min_elements(min_elements),
i_max_elements(max_elements),
i_elements(3),  //costruttore associato al costruttore di Chain
b_first(true),
b_last(false),
b_lmin(true),
b_lmax(false)
{
	double theta_max = M_PI-2*asin((dist_ex/(2.0*radius)));	//ottenuto da osservazioni trigonometriche				
    d_K = cos(theta_max);
}

MyRandom::~MyRandom()
{
}

double MyRandom::Random_theta() //theta distr. tra 0-Pi sinusoidalmente
{
	double u = Rndm();
	u = 2*u-1;
	return acos(u);
}

double MyRandom::Random_thetaVex() //theta distr. tra 0-theta_max sinusoidalmente
{
	double u = Rndm();
	u = u*(1-d_K) + d_K;
	return acos(u);
}

double MyRandom::Random_phi() //phi distribuito tra 0-2Pi uniforme
{
	double u = Rndm();
	return u*2*M_PI;
}

// A = aggiungi, R = rimuovi, D = a destra, S = a sinistra, NULL = non fare nulla
string MyRandom::Choose_action() //provvisorio
{	
	string s_action("00"); //azione non indicata
	double rnd1 = Rndm();
	double rnd2 = Rndm();
	
	if (rnd1 < d_probAdd && !b_lmax) s_action[0]='A';      //se voglio Add e la lunghezza non è massima
	else if(rnd1 < d_probAdd && b_lmax) s_action="NULL";   //se lo è non faccio nulla
	else if(rnd1 >= d_probAdd && !b_lmin) s_action[0]='R'; //se voglio Rm e la lungh nn è minima
	else if(rnd1 >= d_probAdd && b_lmin) s_action="NULL";  //se lo è non faccio nulla

	if(s_action[0]=='A'){
		if(b_first) s_action[1]='D';
		else if(b_last) s_action[1]='S';
		else if(rnd2 < d_probSx) s_action[1]='S';
		else if(rnd2 >= d_probSx) s_action[1]='D';
	}

	else if(s_action[0]=='R'){		//controllo non sia NULL
		if(rnd2 < d_probSx) s_action[1]='S';
		else s_action[1]='D';
	}

	if(s_action=="00") std::cout <<"WARNING: no action choosed"<<endl;  //DEBUG

	return s_action;
}

void MyRandom::Update_state(string s_action_done, int i_name) //i_name: Ca Added or Rmved
{
	if(s_action_done=="NULL") return; //se non ho fatto nulla lo stato rimane tale

	else if(s_action_done[0]=='A')
	{
		i_elements++;
		if(i_elements==i_max_elements) b_lmax = true; //sono a lung massima?
		b_lmin = false; //se aggiungo non sono più a lunghezza minima
		
		if(i_name==0) b_first = true; //ho aggiunto 0?
		if(i_name==i_max_elements-1) b_last = true;  //ho aggiunto 65?
	}

	else if(s_action_done[0]=='R')
	{
		i_elements--;
		if(i_elements==i_min_elements) b_lmin = true; //sono a lung massima?
		b_lmax = false; //se rimuovo non sono più a l max
		
		if(i_name==0) b_first = false; //ho rimosso 0?
		if(i_name==i_max_elements-1) b_last = false;  //ho rimosso 65?
	}

	return;
}