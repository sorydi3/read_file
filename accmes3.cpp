// accmes3.cpp : Defines the entry point for the console application.
//
#include"stdafx.h"
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
using namespace std;

const int CAPACITY = 100;
struct Vector_enter {
	unsigned T[CAPACITY];
	unsigned current_size;
};

struct Variables {
	unsigned paraules;
	unsigned separadors;
	unsigned lletres;
};
bool es_separador(char lletra) {
	/*Pre: cert
	Post: retorna cert si lletra és un separador, fals altrament */
	bool separador = true;
	if (lletra >= 'a' && lletra <= 'z') separador = false;
	else if (lletra >= 'A' && lletra <= 'Z') separador = false;
	//else if (lletra >= '0' && lletra <= '9') separador = false; 
	//else if (lletra == EOS) separador = false; //només si susa marca de fi lògica 
	else { //considerem els caràcters especials catalans i castellans 
		switch (lletra) {
			//case '·':  
		case 'ç': case 'Ç': case 'ñ': case'Ñ': case 'à': case 'á':
		case 'è': case 'é': case 'í': case'ò': case 'ó': case 'ú': case 'ï':
		case 'ü':
			separador = false; break;
		}
	}
	return separador;
}

void passar_separadors(ifstream &in_file,Variables &separador) {
	/*Pre: fitxer obert
	Post: shan llegit tots els separadors del fitxer*/
	char lletra;
	in_file.get(lletra);
	while (!in_file.eof() && es_separador(lletra)) {
		separador.separadors++;
		lletra=in_file.get();
	}
	//separador.separadors- 1;
	if(!in_file.eof())in_file.unget(); //pot no ser necessari 
}

void passar_paraula(Vector_enter &histo,ifstream &in_file, Variables &resume) {
	/*Pre: fitxer vàlid i apunt de llegir el 1r carácter duna paraula
	Post: sha llegit tota una paraula del fitxer */
	char lletra = in_file.get();
	unsigned countador = 0;
	if (!es_separador(lletra)) resume.paraules++;
	while ( !in_file.eof() && !es_separador(lletra)) {
		//tractar, si cal, el caràcter llegit (lletra) 
		countador++;
		resume.lletres++;
		in_file.get(lletra);
	}
	histo.T[countador]++;
	if(!in_file.eof()) in_file.unget(); //pot no ser necessari 
}


void llegir_fitxer(ifstream &in_file, Vector_enter &histo,Variables &resum) {
	//pre:cert
	//post:0<=n<=CAPACITY vec[0..n-1] con n valors entrats
		passar_separadors(in_file, resum);
	while (!in_file.eof()) {
		passar_paraula(histo,in_file, resum);
		passar_separadors(in_file, resum);
	}
}
void inicialitza_vector(Vector_enter &vector) {
	for (unsigned i = 0; i < vector.current_size; i++){
	vector.T[i] = 0;
}
}
unsigned retornPosicioMinim(Vector_enter &vector) {
	bool trobat = false;
	unsigned countador = 0;
	while (!trobat && countador< vector.current_size) {
		if (vector.T[countador] >= 1) trobat = true;
		countador++;
	}
	if(trobat)return countador;
	else return 0;
}
void mostra_vector(Vector_enter& vector, Variables variables) {
	//double paraules = double(variables.paraules);
	double paraules = static_cast<double>(variables.paraules);
	unsigned mini_posicio = retornPosicioMinim(vector)-1;
	if (mini_posicio != 0)
		for (unsigned i = mini_posicio; i < vector.current_size; i++) cout << "[" << i << "]" << " = " <<fixed<< setprecision(3) << (vector.T[i] /paraules) * 100 << setw(8) << right << endl;
	else cout << "fitxer sence paraules " << endl;
}


int main()

{
	Variables myVariables;
	myVariables.lletres = 0;
	myVariables.paraules = 0;
	myVariables.separadors = 0;
	Vector_enter histo;
	histo.current_size = 45;

	//obro el fitxer
	cout << "NOM FITXER:" << endl;
	string nom_fitxer;
	cin >> nom_fitxer;
	ifstream in_file(nom_fitxer.c_str());
	if (in_file.is_open()) {
		inicialitza_vector(histo);
		llegir_fitxer(in_file, histo, myVariables);
		cout <<"CARÀCTERS: " << myVariables.lletres + myVariables.separadors << endl;
		cout << "LLETRES: " << myVariables.lletres << endl;
		cout << "MOTS: " << myVariables.paraules << endl;
		mostra_vector(histo, myVariables);
	}

	else {
		cout << "FITXER INEXISTENT" << endl;
	}
	return 0;
}