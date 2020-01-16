#include "Network.h"
#include <iostream>

//NOTA: meglio ciclare fissando le righe, più efficente
Network::Network(int elNumb):
i_elNumb(elNumb),
i_total_links(0)
{
	ad_matrix = new int*[i_elNumb];
	ad_matrix[0] = new int[i_elNumb*i_elNumb];
	
	for(int j = 1; j < i_elNumb; j++) ad_matrix[j] = ad_matrix[j-1] + i_elNumb; //aritmetica di puntatori
	
	for(int i = 0; i < i_elNumb; i++){	
		for (int j = 0; j < i_elNumb; ++j){
			ad_matrix[i][j] = 0;
		}
	}
}

Network::~Network() 
{
	delete[] ad_matrix[0];
	delete[] ad_matrix;
}

void Network::Show_matrix() const
{	
	for(int i = 0; i < i_elNumb; i++){	
		for (int j = 0; j < i_elNumb; ++j){
			std::cout << ad_matrix[i][j] <<" ";
		}
		std::cout<<endl;
	}
	return;
}

void Network::Add_contacts(int name, vector<int> contacts)
{
	int i;
	i_total_links += contacts.size();
	while(!contacts.empty()) //finchè non è vuoto: inserisco il contatto e rimuovo
	{
		i=contacts.back();
		ad_matrix[name][i]=1;
		ad_matrix[i][name]=1;
		contacts.pop_back();
	}
	return;
}

void Network::Remove_contacts(int name)
{
	for (int i = 0; i < i_elNumb; i++)
	{
		if(ad_matrix[name][i]==1){
			ad_matrix[name][i]=0;
			i_total_links--;
		}
	}

	for (int j = 0; j < i_elNumb; j++) ad_matrix[j][name]=0; //libero anche la colonna
	return;
}

vector<int> Network::Find_correct_contacts(int name, vector<int> contacts) const
{
	int i;
	vector<int> correct_contacts;
	while(!contacts.empty()) //finchè non è vuoto: inserisco il contatto e rimuovo
	{
		i=contacts.back();
		if(ad_matrix[name][i]==1) correct_contacts.push_back(i);
		contacts.pop_back();
	}
	return correct_contacts;
}

int Network::Count_contacts(int name) const
{
	int i_numb_contacts=0;
	for (int i = 0; i < i_elNumb; i++) i_numb_contacts+=ad_matrix[name][i];

	return i_numb_contacts;
}
