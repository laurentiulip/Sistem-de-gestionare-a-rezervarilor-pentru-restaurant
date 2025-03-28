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
protected:
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

    virtual ~Rezervare() {}

    void afisareInfo() const {
        
        cout << "Tip: " << getTip() << endl;
        cout << "Client: " << numeClient << endl;
        cout << "Data si ora: " << dataOra << endl;
        cout << "Numar persoane: " << nrPersoane << endl;
        cout << "Durata: " << calculeazaDurata() << " ore" << endl;
        cout << "Pret total: " << calculeazaPret() << " lei" << endl;
        cout << "------------------------------" << endl;
    }
};

class RezervareZilnica : public Rezervare {
public:
    int calculeazaPret() const override {
        return nrPersoane <= 2 ? 50 : 50 + (nrPersoane - 2) * 20;
    }

    string getTip() const override { 
        return "Rezervare Zilnica"; 
    }
};

class RezervareEvenimentSpecial : public Rezervare {
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
        return "Rezervare Eveniment Special"; 
    }
};

class RezervareGrupMare : public Rezervare {
public:
    int calculeazaPret() const override {
        return nrPersoane <= 20 ? 500 : 500 + (nrPersoane - 20) * 10;
    }

    string getTip() const override { 
        return "Rezervare Grup Mare"; 
    }
};

class RezervareVIP : public Rezervare {
public:
    int calculeazaPret() const override {
        return nrPersoane <= 5 ? 1000 : 1000 + (nrPersoane - 5) * 50;
    }

    string getTip() const override { 
        return "Rezervare VIP"; 
    }
};

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

        if (tipRezervare == "RezervareZilnica") {
            rezervare = new RezervareZilnica();
        } else if (tipRezervare == "RezervareEvenimentSpecial") {
            int oreSuplimentare;
            if (!(fin >> oreSuplimentare)) {
                cout << "Date insuficiente pentru RezervareEvenimentSpecial!" << endl;
                break;
            }
            RezervareEvenimentSpecial* rezEveniment = new RezervareEvenimentSpecial();
            rezEveniment->setOreSuplimentare(oreSuplimentare);
            rezervare = rezEveniment;
        } else if (tipRezervare == "RezervareGrupMare") {
            rezervare = new RezervareGrupMare();
        } else if (tipRezervare == "RezervareVIP") {
            rezervare = new RezervareVIP();
        } else {
            cout << "Tip rezervare necunoscut: " << tipRezervare << endl;
            continue;
        }

        rezervare->setNumeClient(numeClient);
        rezervare->setDataOra(dataOra);
        rezervare->setNrPersoane(nrPersoane);
        rezervari.push_back(rezervare);
    }

    fin.close();

    for (auto r : rezervari) {
        r->afisareInfo();
        delete r;
    }

    return 0;
}


