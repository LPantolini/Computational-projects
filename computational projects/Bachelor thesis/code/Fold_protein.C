#if !defined(__CINT__) || defined(__MAKECINT__)  
#include <Riostream.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TFile.h>
#include <TCanvas.h>
#include "Vector3D.h"
#include "Atom.h"
#include "Chain.h"
#include "MyRandom.h"
#include "Folder.h"
#endif

const double d_radius = 3.8;
const double d_exclusion = 4.0;    //dist entro la quale si escludono
const double d_interaction = 6.5;  //dist entro la quale si ha interazione
const int min_length = 3;

vector<vector<double>> Read_file(const char *native_file);

void Fold_protein(double d_pAdd=0.49, double d_T=0.9, int N_moves=1e08, int i_seed=1234, bool b_Vex=true,
					bool b_Metropolis=true, bool b_debug=false, const char *native_file="2CI2.txt"){
	
	if(b_Metropolis && !b_Vex){
		cout<<"Invalid option!"<<endl;
		return;
	}

	vector<vector<double>> v_native_coord = Read_file(native_file);
	const int N_atoms = v_native_coord.size(); 
	//costruisco la proteina nativa
	Chain *Native_chain = new Chain(v_native_coord,d_radius,d_exclusion,d_interaction,N_atoms);
	const int N_links = Native_chain->Get_links_numb();
	cout<<"Native structure with: " << N_atoms <<" atoms and "<< N_links <<" contacts" <<endl;
	//costruisco la catena che andrò a piegare
	Chain *My_chain = new Chain(d_radius,d_exclusion,d_interaction,N_atoms);  
	MyRandom *My_random = new MyRandom(i_seed,d_pAdd,min_length,N_atoms,d_radius,d_exclusion);  
	Folder My_folder(d_T,My_random,My_chain,Native_chain,b_Vex,b_Metropolis,b_debug); 

	bool b_action_approved;
	
	int N_trial=1000; //elimino i primi 1000 valori della simulazione
	for (int i = 0; i < N_trial; ++i)
	{
		My_folder.Set_action();
		b_action_approved = My_folder.Test_action();
		if(b_action_approved) My_folder.Perform_action();
	}
	
    
    TH1D *hist_length = new TH1D("hist_length", "Chain length",N_atoms+4, -0.5, N_atoms+3.5);
	TH1D *hist_contacts = new TH1D("hist_contacts", "Chain contacts",N_links, -0.5, N_links-0.5); 

	double *d_t = new double[N_moves];	//per la traccia temporale lunghezze
	double *d_x = new double[N_moves];
	
	int contacts_record=0;  //numero massimo di contatti ottenuti durante la simulazione
	int i_length;
	int i_contacts_numb;
	int j = 1; 				//variabile per caricamento in percentuale
	for (int i = 0; i < N_moves; ++i)
	{
		My_folder.Set_action();
		b_action_approved = My_folder.Test_action();
		if(b_action_approved) My_folder.Perform_action();

		//presa dati
		i_length = My_chain->Get_actual_numb();
		i_contacts_numb = My_chain->Get_links_numb();

		hist_length->Fill(i_length);
		if(N_moves<2e06){  //per evitare tracce troppo pesanti
			d_t[i] = i;
			d_x[i] = static_cast<double>(i_length)/N_atoms;
		}
		
		if(i_length==N_atoms) hist_contacts->Fill(i_contacts_numb);

		if(i_contacts_numb>contacts_record && i_length==N_atoms){
			My_chain->Write_configuration("configuration");
			contacts_record=i_contacts_numb;
		}
	
		if( i == j*(N_moves/100.) ){ //caricamento in percentuale
			cout << j <<'%'<<") length: " << i_length;
			if(b_Metropolis) cout <<" contacts: "<< i_contacts_numb;
			cout << endl;
			j++;
		}
	
	}
	//libero la memoria
	delete Native_chain;
	delete My_chain;
	delete My_random;

	char title[100];
	sprintf(title,"dataT%3.2fP%3.2f.root",d_T,d_pAdd);
	TFile file(title,"recreate");

	if(b_Metropolis){

		TCanvas *c1 = new TCanvas("c1","multipads",1000,600);
		c1->Divide(2,1,0.01,0.01);
		//istogramma lunghezze
		hist_length->SetFillColor(kYellow-7);
		hist_length->SetLineColor(kRed+2);
		c1->cd(1);
		hist_length->DrawNormalized();
		hist_length->Write();
		//istogramma contatti
		hist_contacts->SetFillColor(kRed+1);
		hist_contacts->SetLineColor(kYellow-7);
		c1->cd(2);
		hist_contacts->DrawNormalized();
		hist_contacts->Write();
		//traccia temporale lunghezze
		if(N_moves<2e06){
			TGraph *trace_length = new TGraph(N_moves, d_x, d_t);
			trace_length->SetName("Length_temporal_track");
			trace_length->SetTitle("Length temporal track");
			trace_length->SetLineColor(2);
			trace_length->Write();
		}	
	}

	else
	{
		hist_length->SetFillColor(kYellow-7);
		hist_length->SetLineColor(kRed+2);
		hist_length->DrawNormalized();
		hist_length->Write();
	}
	
  	file.Close();

  	delete[] d_t;
  	delete[] d_x;
 
  	return;

}

vector<vector<double>> Read_file(const char *native_file)
{
	double x,y,z;
	vector<vector<double>> v_coordinates;
	ifstream in(native_file);   //classe per le operazioni di input
  	
  	if(!in){ 
    	cout<<"Il file "<<native_file<<" non esiste "<<endl;
    	return v_coordinates;
   	}

	string word;
	while(in>>word)
	{
		if(word=="ATOM"){
			string trash;
			in>>trash;//ingnoro un carattere
			in>>word;
			if(word=="CA")
			{
				in >>trash>>trash>>trash; //ignoro tre caratteri
				vector<double> v;
				in>>x>>y>>z;
				v.push_back(x);
				v.push_back(y);
				v.push_back(z);
				v_coordinates.push_back(v);	
			}
		}
	}

    return v_coordinates;
}