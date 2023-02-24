
// PROIECT POO - Alexandru Robert-Mihai

/*
  TITLUL PROIECTULUI - Gestiunea Conturilor Bancare
 - deschidere de cont |
 - depuneri / retrageri de bani |
 - inchidere cont/uri |
 - transfer de bani intre conturi
 - calcul total pentru mai multe conturi (ale unei persoane)
*/

#include <iostream>
#include <string>
#include <windows.h> //pentru activarea sleep-ului
#include <stdlib.h> //pentru culori in proiect / linux users + altele
#include <cstdlib>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <list>

using namespace std;
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
void intro();
void meniu();

// Avem de realizat 3 clase prin implementarea conceptului has a / is a :
// Banca (has) -> Client (is) -> Cont (has a)- abstracta -> Economi,Credit,Curent;

class Cont;
class ContCurent;
class ContEconomii;
class ContCredit;

class Exceptie {
private:
	string mesajExceptie;
public:
	Exceptie(string mesajExceptie) {
		this->mesajExceptie = mesajExceptie;
	}
	string getMesajExceptie() {
		return mesajExceptie;
	}
};

class Client {
private:
	const int contor;
	static int nrClienti;
	char* nume;
	string prenume;
	long long int CNP;
	char* nrTelefon;
	float soldClient;
	bool varstaRestrictie;
	string email;
	vector<Cont*> conturi;
public:
	// constructor fara parametrii
	Client() : contor(nrClienti++) {
		nume = new char[strlen("Anonim") + 1];
		strcpy(nume, "Anonim");
		prenume = "Anonim";
		CNP = 0;
		nrTelefon = new char[strlen("0000000000") + 1];
		strcpy(nrTelefon, "0000000000");
		soldClient = 0.0;
		varstaRestrictie = false;
		email = "-";
	}
	// constructor cu parametrii (implementare noi clienti)
	Client(const char* nume, string prenume, long long int CNP, const char* nrTelefon, float soldClient,
		bool varstaRestrictie, string email) : contor(nrClienti++) {
		this->nume = new char[strlen(nume) + 1];
		strcpy(this->nume, nume);
		this->prenume = prenume;
		this->CNP = CNP;
		this->nrTelefon = new char[strlen(nrTelefon) + 1];
		strcpy(this->nrTelefon, nrTelefon);
		this->soldClient = soldClient;
		this->varstaRestrictie = varstaRestrictie;
		this->email = email;
	}
	// constructor de copiere
	// la copiere nu mai trb sa fac verificare la char*

	Client(const Client& c) :contor(nrClienti++) {
		if (c.nume != NULL) {
			this->nume = new char[strlen(c.nume) + 1];
			strcpy(this->nume, c.nume);
		}
		else {
			this->nume = NULL;
		}
		prenume = c.prenume;
		CNP = c.CNP;
		if (c.nrTelefon != NULL) {
			this->nrTelefon = new char[strlen(c.nrTelefon) + 1];
			strcpy(this->nrTelefon, c.nrTelefon);
		}
		else {
			nrTelefon = NULL;
		}
		soldClient = c.soldClient;
		varstaRestrictie = c.varstaRestrictie;
		email = c.email;
	}

	// operatorul =
	Client operator=(const Client& c) {
		if (c.nume != NULL) {
			if (this->nume != NULL) {
				delete[] this->nume;
			}
			this->nume = new char[strlen(c.nume) + 1];
			strcpy(this->nume, c.nume);
		}
		else {
			this->nume = NULL;
		}
		prenume = c.prenume;
		CNP = c.CNP;
		if (c.nrTelefon != NULL) {
			if (this->nrTelefon != NULL) {
				delete[] this->nrTelefon;

			}
			this->nrTelefon = new char[strlen(c.nrTelefon) + 1];
			strcpy(this->nrTelefon, c.nrTelefon);
		}
		else {
			this->nrTelefon = NULL;
		}
		soldClient = c.soldClient;
		varstaRestrictie = c.varstaRestrictie;
		email = c.email;
		return *this;
	}
	// destructor 
	~Client() {
		delete[] nume;
		delete[] nrTelefon;
	}
	// afisare
	friend ostream& operator<<(ostream& out, Client c) {
		out << endl << "------------------------------------------------";
		out << endl << "-> Numele Clientului:  " << c.nume;
		out << endl << "-> Prenumele Clientului: " << c.prenume;
		out << endl << "-> CNP: " << c.CNP;
		out << endl << "-> Numarul de Telefon: " << c.nrTelefon;
		out << endl << "-> Sold Client Curent: " << c.soldClient;
		out << endl << "-> Varsta peste 18 ani: " << c.varstaRestrictie;
		out << endl << "-> Email: " << c.email;
		out << endl << "------------------------------------------------";
		return out;
	}
	// introducere noi clienti de la tastatura 
	friend istream& operator>>(istream& in, Client& c) {
		cout << endl << "-------------= Introducere Client =-------------";
		cout << endl << "-> Numele Dumneavoastra: ";
		char aux[20];
		in >> aux;
		c.nume = new char[strlen(aux) + 1];
		strcpy(c.nume, aux);
		cout << endl << "-> Prenumele Dumneavoastra: ";
		in >> c.prenume;

		bool cnpValid = false;
		while (!cnpValid) {
			cout << endl << "-> Introduceti CNP-ul: ";
			in >> c.CNP;
			if (c.CNP < 1000000000000 || c.CNP > 9999999999999) {
				cout << " -> CNP-ul introdus nu este valid. Va rog sa introduceti un CNP valid." << endl;
			}
			else {
				cnpValid = true;
			}
		}
		cout << endl << "-> Introduceti numarul dumneavoastra de telefon!";
		cout << endl << "-> Atentie programul permite un numar maxim de 10 caractere" << endl;
		bool numarulValid = false;
		while (!numarulValid) {
			cout << endl << "-> Numarul de Telefon : ";
			char aux2[15];
			in >> aux2;
			if (strlen(aux2) < 10) {
				cout << "-> Numarul de telefon introdus este gresit introdus. Va rog introduceti un numar de maxim 10 cifre." << endl;
			}
			else {
				if (c.nrTelefon != NULL) {
					delete[] c.nrTelefon;
				}
				c.nrTelefon = new char[strlen(aux2) + 1];
				strcpy(c.nrTelefon, aux2);
				numarulValid = true;
			}
		}
		cout << endl << "-> PRIMUL DEPOZIT IN BALANTA CONTULUI DUMNEAVOASTRA" << endl;
		cout << endl << "-> Atentie, suma minima este de 100 de lei!" << endl;
		cout << endl << "-> Suma: ";
		in >> c.soldClient;
		while (c.soldClient < 100) {
			cout << endl << "-> Suma introdusa este prea mica. Introduceti o suma de cel putin 100 lei: ";
			in >> c.soldClient;
		}

		cout << endl << "-> Varsta Dumneavoastra este peste 18 ani?" << endl;
		cout << endl << "( da / nu) : ";
		string raspuns;
		in >> raspuns;
		if (raspuns == "da") {
			c.varstaRestrictie = true;
			cout << "Da";
		}
		else {
			c.varstaRestrictie = false;
			system("cls");
			cout << "Nu, din pacate nu veti putea inainta in procesul de creeare a contului bancar";
			cout << endl << "-> Va multumim!";
			Sleep(5000);
			meniu();
		};
		cout << endl << "-> Email-ul dumneavoastra: ";
		in >> c.email;
		cout << endl << "-------------= Creearea Contului Bancar =-------------";
		return in;
	}
	// getter pentru variabilele din private
	char* getNume() {
		return nume;
	}
	string getPrenume() {
		return prenume;
	}
	long long int getCNP() {
		return CNP;
	}
	char* getNrTelefon() {
		return nrTelefon;
	}
	float getSoldClient() {
		return soldClient;
	}
	bool getVarstaRestrictie() {
		return varstaRestrictie;
	}
	string getEmail() {
		return email;
	}
	// setter pentru variabilele din private
	void setNume(const char* nume) {
		if (this->nume != NULL) {
			delete[] this->nume;
		}
		this->nume = new char[strlen(nume) + 1];
		strcpy(this->nume, nume);
	}
	void setPrenume(string prenume) {
		this->prenume = prenume;
	}
	void setCNP(long long int CNP) {
		this->CNP = CNP;
	}
	void setNrTelefon(const char* nrTelefon) {
		if (this->nrTelefon != NULL) {
			delete[] this->nrTelefon;
		}
		this->nrTelefon = new char[strlen(nrTelefon) + 1];
		strcpy(this->nrTelefon, nrTelefon);
	}
	void setSold(float soldClient) {
		this->soldClient = soldClient;
	}
	void setVarstaRestrictie(bool varstaRestrictie) {
		this->varstaRestrictie = varstaRestrictie;
	}
	void setEmail(string email) {
		this->email = email;
	}
	// scoatem din balanta
	Client& operator-=(float valoareScoasa) {
		if (valoareScoasa > soldClient) {
			throw Exceptie("Valoarea introdusa este prea mare! - Fonduri Insuficiente");
		}
		soldClient -= valoareScoasa;
		return *this;
	}
	Client& operator+=(float valoareScoasa) {
		soldClient += valoareScoasa;
		return *this;
	}
	bool operator==(string prenume) {
		return this->prenume == prenume;
	}

	Client operator+=(const Client& c) {
		if (this->prenume == c.prenume) {
			this->soldClient += c.soldClient;
		}
		return *this;
	}

	/*void addContClient(Cont*) {

	}

	int getNrConturi() {
		return nrClienti;
	}
	void adaugaCont(Cont* cc) {
		conturi.push_back(cc);
	}
	void retragere(int cc, float suma) {
		conturi[cc]->retragere(suma);
	}
	void transfer(int contSursa, int contDestinatie, float suma) {
		conturi[contSursa]->transfer(conturi[contDestinatie], suma);
	}*/

};

int Client::nrClienti = 1;
// clasa Client intra in clasa Banca

class Banca {
private:
	char* numeBanca;
	int nrClienti;
	Client* clienti;
public:
	// getter pentru clienti
	Client* getClient() {
		return clienti;
	}

	Banca() {
		numeBanca = new char[strlen("BancaDefault") + 1];
		strcpy(numeBanca, "BancaDefault");
		clienti = new Client[nrClienti + 1];
	}


	Banca operator+=(Client c) {
		this->nrClienti++;
		Client* aux = new Client[nrClienti];
		if (this->clienti != NULL) {
			for (int i = 0; i < nrClienti - 1; i++) {
				aux[i] = this->clienti[i];
			}
			delete[] clienti;
		}
		aux[nrClienti - 1] = c;
		clienti = aux;
		return *this;
	}

	Banca operator -=(Client c) {
		int pozitieClient = -1;
		for (int i = 0; i < nrClienti; i++) {
			if (clienti[i].getPrenume() == c.getPrenume()) {
				pozitieClient = i;
				break;
			}
		}
		if (pozitieClient == -1) {
			throw Exceptie("Clientul nu a fost gasit!");
		}
		else {
			for (int i = pozitieClient; i < nrClienti - 1; i++) {
				clienti[i] = clienti[i + 1];
			}
			nrClienti--;
		}
		return *this;
	}
	//functia de afisare a utilizatorului din banca

	friend ostream& operator<<(ostream& out, Banca b) {
		out << endl << "----------------------------------------------";
		out << endl << "-> Numarul Clientilor Inregistrati: " << b.nrClienti;
		out << endl << "----------------------------------------------";
		for (int i = 0; i < b.nrClienti; i++) {
			out << endl << b.clienti[i];
		}
		return out;
	}

	// introducerea a noi clienti
	friend istream& operator>>(istream& in, Banca& b) {
		cout << "Introduceti numarul de clienti pe care ii doriti: ";
		in >> b.nrClienti;
		b.clienti = new Client[b.nrClienti];
		for (int i = 0; i < b.nrClienti; i++) {
			in >> b.clienti[i];
		}
		return in;
	}

	// index pentru verificarea clientului de la o anumita pozitie
	Client& operator[](int index) {
		if (index >= 0 && index < this->nrClienti) {
			return clienti[index];
		}
		else {
			cerr << ("-> Clientul pe care doriti sa-l cautati, nu exista!");
		}
	}


	~Banca() {
		delete[] numeBanca;
	}

	Client gasesteClientDupaPrenume(string prenume) {
		for (int i = 0; i < nrClienti; i++) {
			if (clienti[i].getPrenume() == prenume) {
				return clienti[i];
			}
		}
		throw Exceptie("-> Clientul cautat nu a fost gasit! ");
	}
	// acelasi lucru doar ca e prin supraincarcare la [ ];
	Client& operator[](string prenume) {
		for (int i = 0; i < nrClienti; i++) {
			if (clienti[i].getPrenume() == prenume) {
				return clienti[i];
			}
		}
		throw Exceptie("Clientul nu a fost gasit!");
	}
	int getNrClienti() const {
		return nrClienti;
	}
	float getSoldTotalClient(string prenume) {
		Client total;
		for (int i = 0; i < nrClienti; i++) {
			if (clienti[i].getPrenume() == prenume) {
				total += clienti[i];
			}
		}
		return total.getSoldClient();
	}
	// functii de afisare in fisierul text
	friend ofstream& operator<<(ofstream& ofs, Banca& b) {
		if (b.nrClienti > 0) {
			ofs << "->Clienti: " << endl;
			for (int i = 0; i < b.nrClienti; i++) {
				ofs << b.clienti[i] << endl;
			}
		}
		return ofs;
	}
	friend ifstream& operator>>(ifstream& ifs, Banca& b) {
		b.clienti = new Client[b.nrClienti];
		for (int i = 0; i < b.nrClienti; i++) {
			ifs >> b.clienti[i];
		}
		return ifs;
	}

};
// implementarea unor conturi functii virtuale pure si unei clase abstracte Cont legata de Clienti
class Cont {
private:
	float dobanda;
	string nume;
public:
	Cont() :nume("Anonim"), dobanda(0.0) {}
	Cont(float dobanda) {
		this->dobanda = dobanda;
	}

	virtual void depunere(Client& c, float suma) = 0;
	virtual void retragere(float suma) = 0;
	virtual float getSold() = 0;
	virtual void setSold(float soldClient) = 0;
	virtual void transfer(Cont* cont, float suma) = 0;
	virtual void setClient(Client& c) = 0;
	virtual Client& getClient() = 0;

	virtual void operator+=(float suma) {
		retragere(suma);
	};
};

// implementarea claselor mostenite

class ContEconomii : public Cont {
private:
	float suma;
	bool activ;
	float dobanda;
public:

	bool getActiv() {
		return activ;
	}
	void setActiv(bool activ) {
		this->activ = activ;
	}
};
class ContCurent : public Cont {
private:
	bool activ;

public:
	bool getActiv() {
		return activ;
	}
	void setActiv(bool activ) {
		this->activ = activ;
	}
};
class ContCredit : public Cont {
private:
	bool activ;

public:
	bool getActiv() {
		return activ;
	}
	void setActiv(bool activ) {
		this->activ = activ;
	}
};

int main() {

	// initializare banca si clienti
	Banca b;
	Client c, clientGasit, clientGasit3;
	int optiune;
	string prenume, prenume1, prenume2;

	intro();
	system("cls");
	do {
		meniu();
		cin >> optiune;
		switch (optiune) {
		case 1:
			system("cls");
			cin >> b;
			cout << endl << endl << " -> Clientul a fost introdus in banca cu succes!";
			Sleep(3000);
			break;
		case 2:
			char optiune2;
			float valoareAdaugata;
			system("cls");
			while (true) {
				cout << "-> Introduceti optiunea dorita: ";
				cout << endl << "*-> d. Depozitarea unei sume de bani !";
				cout << endl << "*-> r. Retragerea unei sume de bani !";
				cout << endl << "*-> x. Anulare, revenire in meniul principal! (EXIT)";
				cout << endl << "-> Optiunea aleasa: ";
				cin >> optiune2;
				if (optiune2 == 'x') {
					break;
				}
				else {
					cout << endl << "-> Introduceti valoare pe care doriti sa o utilizati: ";
					cin >> valoareAdaugata;
					cout << endl << "-> Introduceti prenumele complet al clientului pe care doriti sa-l cautati: ";
					cin >> prenume;
					clientGasit = b.gasesteClientDupaPrenume(prenume);
					try {
						if (optiune2 == 'd') {
							clientGasit += valoareAdaugata;
							cout << endl << "-> Valoarea de " << valoareAdaugata << " a fost adaugata cu succes!";
						}
						else if (optiune2 == 'r') {
							clientGasit -= valoareAdaugata;
							cout << endl << "-> Valoarea de " << valoareAdaugata << " a fost retrasa cu succes!";
						}
						cout << "Detaliile contului clientului cautat sunt: " << endl;
						cout << clientGasit;
					}
					catch (Exceptie e) {
						cout << endl << e.getMesajExceptie() << endl;
					}
				}
			}
			break;
		case 3:
			system("cls");
			float valoareAdaugataTransfer;
			char optiune3;
			while (true) {
				cout << endl << "-> Bun venit, la transferul dintre clienti";
				cout << endl << "---=> INSTRUCTIUNI:";
				cout << endl << "> Introduceti prenumele primului client de la care se transfera banii din cont urmatorului client; ";
				cout << endl << "> Introduceti prenumele celuilalt client caruia i se vor restituii banii;";
				cout << endl << "-> Prenumele primului client: ";
				cin >> prenume1;
				clientGasit = b.gasesteClientDupaPrenume(prenume1);
				cout << endl << "-> Prenumele celui de-al doilea client";
				cin >> prenume2;
				clientGasit3 = b.gasesteClientDupaPrenume(prenume2);
				cout << endl << "-> Introduceti valoare pe care doriti sa o utilizati: ";
				cin >> valoareAdaugataTransfer;
				cout << endl << "-> Alegeti una dintre urmatoarele optiuni: ";
				cout << endl << "> 't' - pentru a transmite suma de bani introdusa;";
				cout << endl << "> 'x' - pentru a anula tranzactia si revenii in meniul proiectului principal;";
				cin >> optiune3;

				try {
					if (optiune3 == 'x') {
						break;
					}
					else if (optiune3 == 't') {
						clientGasit -= valoareAdaugataTransfer;
						clientGasit3 += valoareAdaugataTransfer;
						cout << endl << "Valoarea " << valoareAdaugataTransfer << " a fost adaugata cu succes!";
					}
					cout << endl << "Detaliile contului clientuli carei i s-a facut transferul sunt: " << endl;
					cout << clientGasit3;
				}
				catch (Exceptie e) {
					cout << endl << e.getMesajExceptie() << endl;
				}
			}
			break;
		case 4:
			system("cls");
			while (true) {
				try {
					string prenume;
					cout << "-> Introduceti prenumele clientului ce urmeaza a fi sters: ";
					cin >> prenume;
					Client c = b[prenume];
					b -= b[prenume];
					cout << "-> Clientul " << c.getPrenume() << " a fost sters ! " << endl;
					break;
				}
				catch (Exceptie& e) {
					cout << e.getMesajExceptie() << endl;
					char raspuns;
					cout << "-> Doriti sa continuati stergerea? (d/n): ";
					cin >> raspuns;
					if (raspuns == 'n') {
						break;
					}
				}
			}
			Sleep(3000);
			break;
		case 5:
			system("cls");
			for (int i = 0; i < b.getNrClienti(); i++) {
				c = b[i];
				cout << c << endl;
			}
			Sleep(5000);
			break;
		case 6:
			system("cls");
			while (true) {
				cout << "-> Introduceti prenumele clientului pentru care doriti sa vedeti soldul total: ";
				string prenume;
				cin >> prenume;
				try {
					double soldTotal = b.getSoldTotalClient(prenume);
					cout << "-> Soldul total al clientului " << prenume << " este: " << soldTotal << endl;
				}
				catch (Exceptie& e) {
					cout << e.getMesajExceptie() << endl;
				}
				cout << "-> Introduceti '1' pentru a cauta din nou sau orice alt caracter pentru a iesi: ";
				char optiune;
				cin >> optiune;
				if (optiune != '1') break;
			}
			break;
		case 7:
			system("cls");
			while (true) {
				try {
					string prenume;
					cout << "-> Introduceti prenumele clientului: ";
					cin >> prenume;
					Client c = b[prenume];
					cout << "-> Clientul a fost gasit: " << c << endl;
					break;
				}
				catch (Exceptie& e) {
					cout << e.getMesajExceptie() << endl;
					char raspuns;
					cout << "-> Doriti sa continuati cautarea? (d/n): ";
					cin >> raspuns;
					if (raspuns == 'n') {
						break;
					}
				}
			}
			Sleep(3000);
			break;
		case 8:
		{system("cls");
		cout << endl << "-> Atentie fisierul trebuie numit - fisierClienti.txt - pentru a functiona !";
		ofstream fileOut("fisierClienti.txt", ios::out);
		fileOut << b;
		fileOut.close();
		cout << "-> Scrierea in fisierul text a fost realizata cu succes !";
		Sleep(3000);
		}

		break;

		case 9: {
			system("cls");
			cout << endl << "-> Introducerea clientilor dintr-un fisier text - fisierClienti.txt - !";
			ifstream fileIn("fisierClienti.txt", ios::in);
			fileIn >> b;
			fileIn.close();
			cout << endl << endl << "-> Clientii au fost introdusi cu succes !";
			Sleep(3000); }
			  break;
		case 10:
			system("cls");
			cout << endl << "Va multumesc pentru utilizarea programului !";
			return 0;
			break;
		default:
			cout << endl << "Din pacate nu exista aceasta optiune introdusa in program!";
			cout << endl << "Va rugam alegeti o optiune de la 1-8 !";
		}
	} while (optiune != 8);  //asta e meniul cu do-while

	return 0;


}

void intro() {
	system("Color B3");
	cout << endl << "| ~*|                                                             |*~ |";
	cout << endl << "|---------------------------------------------------------------------|";
	cout << endl << "|                      GESTIUNEA CONTURILOR                           |";
	Sleep(500);
	cout << endl << "|                             BANCARE                                 |";
	Sleep(500);
	cout << endl << "|---------------------------------------------------------------------|";
	Sleep(500);
	cout << endl << "|             Realizat de : Alexandru Robert-Mihai                    |";
	Sleep(500);
	cout << endl << "|             Grupa: 1048-C                                           |";
	Sleep(500);
	cout << endl << "|    Facultatea de Cibernetica, Statistica si Informatica Economica   |";
	Sleep(500);
	cout << endl << "|---------------------------------------------------------------------|";
	cout << endl << "\tApasati tasta ENTER, pentru a continua... :D";
	cout << endl << endl << "| ~*|                                                             |*~ |";
	cin.get();
}
void meniu() {
	system("cls");
	cout << endl << "*>---------------------~=~--------------------<*";
	cout << endl << "------------~=>  MENIU PRINCIPAL  <=~-----------";
	cout << endl << "*>---------------------~=~--------------------<*";
	cout << endl << "                                                ";
	cout << endl << "---| Bun venit in meniul principal!             ";
	cout << endl << "---| Alegeti una dintre urmatoarele optiuni:    ";
	cout << endl << " *-> Adaugarea unui nou cont bancar          (1)";
	cout << endl << " *-> Depozitarea/Retragerea unei anumite        ";
	cout << endl << "> sume de bani                               (2)";
	cout << endl << " *-> Transfer de bani intre conturi          (3)";
	cout << endl << " *-> Inchiderea unui cont bancar             (4)";
	cout << endl << " *-> Afisarea tuturor conturilor prezente    (5)";
	cout << endl << " *-> Afisarea soldului unui client cu mai       ";
	cout << endl << "multe conturi                                (6)";
	cout << endl << " *-> Afisarea unui anumit client             (7)";
	cout << endl << " *-> Introducerea intr-un fisier text a         ";
	cout << endl << "> clientilor (txt)                           (8)";
	cout << endl << " *-> Introducerea dintr-un fisier text a        ";
	cout << endl << "> clientilor (txt)                           (9)";

	cout << endl << "------------------------------------------------";
	cout << endl << "-----------------=|  EXIT  |=---------------(10)";
	cout << endl << "---| Introduceti o tasta de la (1-10):          ";
}


