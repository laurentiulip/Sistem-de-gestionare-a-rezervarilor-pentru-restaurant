#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Clasa abstractă IRezervare
class IRezervare {
public:
    virtual int calculeazaDurata() const = 0;
    virtual int calculeazaPret() const = 0;
    virtual string getTip() const = 0;
    virtual ~IRezervare() {}
};

// Clasa de bază Rezervare
class Rezervare : public IRezervare {
protected:
    string numeClient;
    string dataOra;
    int nrPersoane;
    int durata;  // Durata rezervării

public:
    // Setter pentru durata
    void setDurata(int durata) {
        this->durata = durata;
    }

    // Getter pentru durata
    int getDurata() const {
        return durata;
    }

    // Setteri pentru celelalte atribute
    void setNumeClient(const string& nume) {
        this->numeClient = nume;
    }

    void setDataOra(const string& data) {
        this->dataOra = data;
    }

    void setNrPersoane(int nr) {
        this->nrPersoane = nr;
    }

    // Afișăm informațiile despre rezervare
    virtual void afisareInfo() const {
        cout << "Tip rezervare: " << getTip() << endl;
        cout << "Client: " << numeClient << endl;
        cout << "Data si ora: " << dataOra << endl;
        cout << "Numar persoane: " << nrPersoane << endl;
        cout << "Durata: " << durata << " ore" << endl;
        cout << "-------------------------------" << endl;
    }

    virtual string getTip() const = 0;  // Metoda virtuală pentru obținerea tipului rezervării

    virtual ~Rezervare() {}
};

// Clasa RezervareZilnica
class RezervareZilnica : public Rezervare {
public:
    RezervareZilnica() {
        setDurata(2);  // Durata implicită pentru o rezervare zilnică este de 2 ore
    }

    string getTip() const override {
        return "Rezervare Zilnica";
    }

    int calculeazaDurata() const override {
        return getDurata();  // Returnăm durata setată
    }

    int calculeazaPret() const override {
        return nrPersoane <= 2 ? 50 : 50 + (nrPersoane - 2) * 20;
    }
};

// Clasa RezervareEvenimentSpecial
class RezervareEvenimentSpecial : public Rezervare {
private:
    int oreSuplimentare = 0;

public:
    // Setăm orele suplimentare și calculăm durata totală
    void setOreSuplimentare(int ore) {
        this->oreSuplimentare = ore;
        setDurata(4 + oreSuplimentare);  // Durata implicită este 4 ore, la care adăugăm orele suplimentare
    }

    string getTip() const override {
        return "Rezervare Eveniment Special";
    }

    int calculeazaDurata() const override {
        return getDurata();  // Returnăm durata setată
    }

    int calculeazaPret() const override {
        return 200 + oreSuplimentare * 50;
    }
};

// Clasa RezervareGrupMare
class RezervareGrupMare : public Rezervare {
public:
    RezervareGrupMare() {
        setDurata(4);  // Durata implicită pentru grupuri mari este de 4 ore
    }

    string getTip() const override {
        return "Rezervare Grup Mare";
    }

    int calculeazaDurata() const override {
        return getDurata();  // Returnăm durata setată
    }

    int calculeazaPret() const override {
        return nrPersoane <= 20 ? 500 : 500 + (nrPersoane - 20) * 10;
    }
};

// Clasa RezervareVIP
class RezervareVIP : public Rezervare {
public:
    RezervareVIP() {
        setDurata(3);  // Durata implicită pentru VIP este de 3 ore
    }

    string getTip() const override {
        return "Rezervare VIP";
    }

    int calculeazaDurata() const override {
        return getDurata();  // Returnăm durata setată
    }

    int calculeazaPret() const override {
        return nrPersoane <= 5 ? 1000 : 1000 + (nrPersoane - 5) * 50;
    }
};

int main() {
    const string path = "rezervari.txt";  // Calea către fișierul de rezervări
    ifstream fin(path);  // Deschidem fișierul pentru citire

    if (!fin.is_open()) {
        cout << "Eroare la deschiderea fisierului " << path << " !" << endl;
        return 1;
    }

    vector<Rezervare*> rezervari;  // Vector pentru stocarea rezervărilor
    string tipRezervare, numeClient, dataOra;
    int nrPersoane, durata;

    while (fin >> tipRezervare >> numeClient >> dataOra >> nrPersoane) {
        Rezervare* rezervare = nullptr;

        // Citim durata pentru fiecare tip de rezervare
        if (tipRezervare == "RezervareZilnica") {
            rezervare = new RezervareZilnica();
            fin >> durata;  // Citim durata specifică
            rezervare->setDurata(durata);
        }
        else if (tipRezervare == "RezervareEvenimentSpecial") {
            int oreSuplimentare;
            fin >> oreSuplimentare;  // Citim orele suplimentare
            RezervareEvenimentSpecial* rezEveniment = new RezervareEvenimentSpecial();
            rezEveniment->setOreSuplimentare(oreSuplimentare);
            rezervare = rezEveniment;
        }
        else if (tipRezervare == "RezervareGrupMare") {
            rezervare = new RezervareGrupMare();
            fin >> durata;
            rezervare->setDurata(durata);
        }
        else if (tipRezervare == "RezervareVIP") {
            rezervare = new RezervareVIP();
            fin >> durata;
            rezervare->setDurata(durata);
        }
        else {
            cout << "Tip rezervare necunoscut: " << tipRezervare << endl;
            continue;
        }

        // Setăm datele clientului
        rezervare->setNumeClient(numeClient);
        rezervare->setDataOra(dataOra);
        rezervare->setNrPersoane(nrPersoane);

        // Adăugăm rezervarea în vectorul de rezervări
        rezervari.push_back(rezervare);
    }

    fin.close();  // Închidem fișierul

    // Afișăm informațiile despre toate rezervările
    for (auto r : rezervari) {
        r->afisareInfo();
        delete r;  // Nu uita să eliberezi memoria
    }

    return 0;
}



