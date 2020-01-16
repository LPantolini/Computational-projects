#ifndef CHAIN_H
#define CHAIN_H

#include "Atom.h"
#include "Network.h"
#include <deque>
#include <string>

class Chain: public Network{

public:

	Chain(double r, double v_ex, double interaction, int max);
	Chain(vector<vector<double>> v_coords, double r, double v_ex, double interaction, int max);  //costruttore per lo stato nativo
	~Chain();

	void Add_element(Atom* Ca,vector<int> contacts);  	
	void Remove_element(int name); 			       
	void Write_configuration(string s_file);       //scrive su s_file.pdb la configurazione in formato pdb 
	void Information_to_Add(string s_side, Vector3D* V, int& name) const; //inserisce le posizioni degli estremi in V
	void Compute_distances(Atom* Ca, vector<int>* v_contacts, bool& v_ex); 
	int Get_extreme_name(string s_side) const;	   //torna il nome di un estremo
	int Get_actual_numb() const {return de_sequence.size();};
	int Get_max_numb() const {return i_max_numb;};
	double Get_radius() const {return d_radius;};
	
private:
	
	const double d_radius;
	const double d_dist_exclusion;  
	const double d_dist_interaction;
	const int i_max_numb;
	deque<Atom*> de_sequence;   

};

#endif