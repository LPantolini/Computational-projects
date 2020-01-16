#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

class Network{

public:

	Network(int elNumb);
	~Network();

	void Show_matrix() const;
	void Add_contacts(int name, vector<int> contacts); //aggiunge i contatti di name
	void Remove_contacts(int name);
	vector<int> Find_correct_contacts(int name, vector<int> contacts) const;
	int Count_contacts(int name) const;   		//ritorna il numero di contatti di un Atom
	int Get_links_numb() const {return i_total_links;};

private:
	
	const int i_elNumb;		//numero righe e colonne
	int i_total_links;
	int **ad_matrix;   		//matrice di adiacenza

};

#endif
