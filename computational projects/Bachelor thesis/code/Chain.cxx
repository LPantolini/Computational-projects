#include "Chain.h"
#include <iostream>

using namespace std;

Chain::Chain(double r, double v_ex, double interaction, int max):
Network(max),			//creo un network con max nodi
d_radius(r),
d_dist_exclusion(v_ex*v_ex),
d_dist_interaction(interaction*interaction),  
i_max_numb(max)     				
{ 
	de_sequence.push_back(new Atom(0.,0.,0.,0));			  //inserisco 3 atomi equidistanti
	de_sequence.push_back(new Atom(0.,0.,d_radius,1));
	de_sequence.push_back(new Atom(0.,d_radius,d_radius,2));
}

Chain::Chain(vector<vector<double>> v_coords, double r, double v_ex, double interaction, int max): 
Network(max),
d_radius(r),    
d_dist_exclusion(v_ex*v_ex),
d_dist_interaction(interaction*interaction),
i_max_numb(max)
{
	for (int i = 0; i < max; ++i)  //inserisco le coordinate
	{
		double x=v_coords[i][0];
		double y=v_coords[i][1];
		double z=v_coords[i][2];

		de_sequence.push_back(new Atom(x,y,z,i));
	}
   	//ora inserisco i links
    deque<Atom*>::iterator first = de_sequence.begin();
	deque<Atom*>::iterator last = de_sequence.end();
	vector<int> v_contacts;

	while(first != last-2)   
	{	
		deque<Atom*>::iterator new_first = first+3;  //interazione senza contare i 2 più vicini
		while(new_first != last)
		{
			Atom Ca_i = *(*new_first);
			double d_dist = (*first)->Squared_distance(Ca_i);
			if(d_dist < d_dist_exclusion) std::cout <<"Error! Wrong model"<<endl;
			else if(d_dist <= d_dist_interaction) v_contacts.push_back((*new_first)->Get_name());
			
			*new_first++;
		}
		Add_contacts((*first)->Get_name(),v_contacts);
		v_contacts.clear();
		
		*first++;
	}

}

Chain::~Chain()
{
	deque<Atom*>::iterator first = de_sequence.begin();
	deque<Atom*>::iterator last = de_sequence.end();

	while(first != last)
	{	
		delete *first;
		*first++;
	}
	
	de_sequence.clear();
}

void Chain::Add_element(Atom* Ca, vector<int> contacts)     //riconosce il lato da solo
{
	int last_name = (de_sequence.back())->Get_name();
	int first_name = (de_sequence.front())->Get_name();

	if(Ca->Get_name() == last_name+1) de_sequence.push_back(Ca);
	else if(Ca->Get_name() == first_name-1) de_sequence.push_front(Ca);
	else cout <<"Error! Tring to add in invalid position" <<endl;

	Add_contacts(Ca->Get_name(), contacts); //aggiorno il network

	return;
}

void Chain::Remove_element(int name)
{	
	int last_name = (de_sequence.back())->Get_name();
	int first_name = (de_sequence.front())->Get_name();

	if(last_name == name){
		delete de_sequence.back();   
		de_sequence.pop_back();
	}

	else if(first_name == name){
		delete de_sequence.front();
		de_sequence.pop_front();
	} 
	else cout <<"Error! Tring to remove a invalid atom " << endl;

	Remove_contacts(name);	//aggiorno il network

	return;
}

void Chain::Write_configuration(string s_file)
{
	string name = s_file +".pdb";
	ofstream os(name);
	os.setf(ios_base::showpoint);   //inserisce gli 0 
	os.precision(5);

	deque<Atom*>::iterator first = de_sequence.begin();
	deque<Atom*>::iterator last = de_sequence.end();

	while(first != last)
	{	
		(*first)->Write_myself(os);
		*first++;
	} 

	os.close();

	return;
}

//passa ciò che è necessario per costruire un novo atom
void Chain::Information_to_Add(string s_side, Vector3D* V, int& name) const
{
	if(s_side[1]=='D')
	{
		name = (de_sequence.back())->Get_name()+1; //de_sequence.size();
		V[0] = *de_sequence[de_sequence.size()-2];	  //penultimo elemento
		V[1] = *de_sequence.back();   //ultimo atom
	}

	else if(s_side[1]=='S')
	{
		name = de_sequence.front()->Get_name()-1;
		V[0] = *de_sequence[1];       //secondo atom
		V[1] = *de_sequence.front(); //primo atom
	}

	return;
}

int Chain::Get_extreme_name(string s_side) const
{
	if(s_side[1]=='D') return de_sequence.back()->Get_name();
	else return de_sequence.front()->Get_name();
}


void Chain::Compute_distances(Atom* Ca, vector<int>* v_contacts, bool& v_ex)
{
	deque<Atom*>::iterator first;
	deque<Atom*>::iterator last;
	if(Ca->Get_name() == de_sequence.back()->Get_name()+1){
		first = de_sequence.begin();
		last = de_sequence.end()-2;
	}

	else{
		first = de_sequence.begin()+2;
		last = de_sequence.end();
	}

	while(first != last)
	{	
		Atom Ca_i = *(*first);
		double d_dist = Ca->Squared_distance(Ca_i);
		if(d_dist < d_dist_exclusion){  //volume escluso: atomo rifiutato
			v_ex = true;
			return;	
		}

		else if(d_dist <= d_dist_interaction) v_contacts->push_back(Ca_i.Get_name());
		
		*first++;
	}
	
	return;
}