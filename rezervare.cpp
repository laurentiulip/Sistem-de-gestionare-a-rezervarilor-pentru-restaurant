#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class IRezervare {
public:
    virtual int calculeazaDurata() const = 0;
    virtual int calculeazaPret() const = 0;
    virtual string getTip() const = 0;
    virtual ~IRezervare() {}
};

class Rezervare : public IRezervare {
public:
    string numeClient;
    string dataOra;
    int nrPersoane;

public:
    void setNumeClient(const string& nume) {
        this->numeClient = nume;
    }

    void setDataOra(const string& data) {
        this->dataOra = data;
    }

    void setNrPersoane(int nr) {
        this->nrPersoane = nr;
    }

    virtual int calculeazaDurata() const override {
        return 2;
    }

    virtual int calculeazaPret() const override {
        return 0;
    }

    virtual string getTip() const override { 
        return "Rezervare"; 
    }
    
    string getNumeClient() { return numeClient; }
    
    string getdataOra() { return dataOra;}
    
    int getnrPersoane() { return nrPersoane;}

    virtual ~Rezervare() {}
};

class Zilnica : public Rezervare {
public:
    int calculeazaPret() const override {
        return nrPersoane <= 2 ? 50 : 50 + (nrPersoane - 2) * 20;
    }

    string getTip() const override { 
        return "Zilnica"; 
    }
};

class EvenimentSpecial : public Rezervare {
private:
    int oreSuplimentare = 0;

public:
    void setOreSuplimentare(int ore) {
        this->oreSuplimentare = ore;
    }

    int calculeazaDurata() const override {
        return 4 + oreSuplimentare;
    }

    int calculeazaPret() const override {
        return 200 + oreSuplimentare * 50;
    }

    string getTip() const override { 
        return "Eveniment Special"; 
    }
};

class GrupMare : public Rezervare {
public:
    int calculeazaPret() const override {
        return nrPersoane <= 20 ? 500 : 500 + (nrPersoane - 20) * 10;
    }

    string getTip() const override { 
        return "Grup Mare"; 
    }
};

class VIP : public Rezervare {
public:
    int calculeazaPret() const override {
        return nrPersoane <= 5 ? 1000 : 1000 + (nrPersoane - 5) * 50;
    }

    string getTip() const override { 
        return "VIP"; 
    }
};

void afisareInfo(vector<Rezervare*> rezervari){
    cout << "Tip\t\t" << "Client\t\t" << "DataOra\t\t\t" << "Nr persoane\t\t" << "Durata\t\t" << "Pret total\t\t" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------------" << endl;
    int count = 1;
    for(auto rezervare : rezervari)
    {
        cout << count << ". " << rezervare->getTip() << "\t" << rezervare->getNumeClient() << "\t\t" << rezervare->getdataOra() << "\t" <<
        rezervare->getnrPersoane() << "\t\t\t" << rezervare->calculeazaDurata() << "\t\t" << rezervare->calculeazaPret() << endl;
        count++;
    }
}

int main() {
    const string path = "rezervari.txt";
    ifstream fin(path);
    if (!fin.is_open()) {
        cout << "Eroare la deschiderea fisierului " << path << " !" << endl;
        return 1;
    }

    vector<Rezervare*> rezervari;
    string tipRezervare, numeClient, dataOra;
    int nrPersoane;

    while (fin >> tipRezervare >> numeClient >> dataOra >> nrPersoane) {
        Rezervare* rezervare = nullptr;

        if (tipRezervare == "Zilnica") {
            rezervare = new Zilnica();
        } else if (tipRezervare == "EvenimentSpecial") {
            int oreSuplimentare;
            if (!(fin >> oreSuplimentare)) {
                cout << "Date insuficiente pentru Eveniment Special!" << endl;
                break;
            }
            EvenimentSpecial* rezEveniment = new EvenimentSpecial();
            rezEveniment->setOreSuplimentare(oreSuplimentare);
            rezervare = rezEveniment;
        } else if (tipRezervare == "GrupMare") {
            rezervare = new GrupMare();
        } else if (tipRezervare == "VIP") {
            rezervare = new VIP();
        } 

        rezervare->setNumeClient(numeClient);
        rezervare->setDataOra(dataOra);
        rezervare->setNrPersoane(nrPersoane);
        rezervari.push_back(rezervare);
    }
    
    afisareInfo(rezervari);
    
    fin.close();

    return 0;
}


