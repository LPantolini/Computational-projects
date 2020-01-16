#include "Folder.h"
#include <cmath>
#include <iostream>

Folder::Folder(double T,MyRandom *random,Chain* chain,Chain* native,bool Vex,bool Metropolis,bool debug):
d_T(T),
s_action("00"),
b_Vex(Vex),
b_Metropolis(Metropolis),
b_debug(debug),
pnt_random(random),
pnt_mychain(chain),
pnt_native(native)
{
}

Folder::~Folder()
{
}

void Folder::Set_action()
{
	s_action = pnt_random->Choose_action();
	if(s_action[0]=='A') pnt_new_Ca = Craft_Atom();
	else if(s_action[0]=='R') i_Ca_name = pnt_mychain->Get_extreme_name(s_action);
	
	if(b_debug) std::cout << s_action << endl;
	return;
}

bool Folder::Test_action()
{
	bool b_approved = true;
	if(!b_Vex && !b_Metropolis) return b_approved;

	if(s_action[0]=='A'){
		v_contacts.clear();
		bool b_excluded = false;
		pnt_mychain->Compute_distances(pnt_new_Ca, &v_contacts, b_excluded);

		if(b_excluded){	//se sono nel volume escluso non approvo e cancello la memoria allocata
			b_approved = false;
			delete pnt_new_Ca;
			return b_approved;
		}

	}

	//Metropolis
	if(b_Metropolis){
		double d_rand = pnt_random->Rndm();
		int i_dE = 0;  //delta energetico

		if(s_action[0]=='A'){
			v_contacts = pnt_native->Find_correct_contacts(pnt_new_Ca->Get_name(), v_contacts);
			i_dE = -v_contacts.size();
		}
		else if(s_action[0]=='R') i_dE = pnt_mychain->Count_contacts(i_Ca_name);

		if( (-i_dE)/d_T >= log(d_rand) ) b_approved = true;  //test Metropolis
		
		else{
			b_approved=false;
			if(s_action[0]=='A') delete pnt_new_Ca;	
		} 
	}

	if(b_debug) std::cout <<"Azione " << b_approved << endl;
	
	return b_approved;
}

void Folder::Perform_action()
{
	if(s_action[0]=='A'){
		pnt_mychain->Add_element(pnt_new_Ca,v_contacts);
		pnt_random->Update_state(s_action, pnt_new_Ca->Get_name());
		if(b_debug) std::cout <<"Aggiunto "<< pnt_new_Ca->Get_name() << endl;
	}
	else if(s_action[0]=='R'){ 
		pnt_mychain->Remove_element(i_Ca_name);
		pnt_random->Update_state(s_action,i_Ca_name);
		if(b_debug) std::cout <<"Tolto "<< i_Ca_name << endl;
	}

	s_action = "00";
	return;
}

Atom* Folder::Craft_Atom()  
{
	int name = 0;
	Vector3D v_V[2];
	pnt_mychain->Information_to_Add(s_action,v_V,name);

	Vector3D v_ext = v_V[1];	//v_ext atom esterno
	Vector3D v_int = v_V[0];

	//calcolo gli assi del nuovo Ca
	Vector3D v_Z = v_ext-v_int;
	if(v_Z.Norm()!=0) v_Z = v_Z*(1./v_Z.Norm());  
	
	Vector3D v_Y = v_int^v_ext;
	if(v_Y.Norm()!=0) v_Y = v_Y*(1./v_Y.Norm());

	Vector3D v_X = v_Y^v_Z;
	//genero gli angoli nel sistema centrato nell'ultimo CA
	double theta;
	if(b_Vex) theta = pnt_random->Random_thetaVex();  //smart sampling
	else theta = pnt_random->Random_theta(); 
	double phi = pnt_random->Random_phi();
	double d_R = pnt_mychain->Get_radius();

	Vector3D v_new_Z = v_X*cos(phi)*sin(theta) + v_Y*sin(phi)*sin(theta) + v_Z*cos(theta);
	Vector3D v_new_Ca = v_ext + v_new_Z*d_R;
	
	Atom* Ca = new Atom(v_new_Ca,name);
	
	return Ca;

}
