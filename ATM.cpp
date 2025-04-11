#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

class Cont {
public:
    int id;
    string pas;
    string nume;
    float sold;
    vector<string> tranzactii;
    
    // Constructor
    Cont(int id, const string& nume, float sold, const string&pas) : id(id), nume(nume), sold(sold), pas(pas){}

    // Depunere bani
    void depuneBani(float suma) {
        sold += suma;
        tranzactii.push_back("Depunere: +" + to_string(suma));
    }

    // Retragere bani
    bool retrageBani(float suma) {
        if (sold >= suma) {
            sold -= suma;
            tranzactii.push_back("Retragere: -" + to_string(suma));
            return true;
        }
        return false;
    }
    
    // Transfer bani către alt cont
    bool trimiteBani(Cont* destinatie, float suma) {
        if (sold >= suma) {
            sold -= suma;
            destinatie->sold += suma;
            tranzactii.push_back("Transfer catre cont " + to_string(destinatie->id) + ": -" + to_string(suma));
            destinatie->tranzactii.push_back("Transfer de la cont " + to_string(id) + ": +" + to_string(suma));
            return true;
        }
        return false;
    }

    // Vizualizare tranzacții
    void vizualizeazaTranzactii() {
        cout << "\nIstoric tranzactii pentru contul " << id << ":\n";
        for (const auto &tranzactie : tranzactii)
            cout << tranzactie << endl;
    }

    // Blocare cont
    void blocheazaCont() {
        tranzactii.push_back("Cont blocat.");
    }
};

// Vector pentru gestionarea conturilor
vector<Cont*> conturi;

// // Funcție pentru "curățarea" consolei prin afișarea unui număr mare de linii noi
//  void clearConsole() {
//      cout << string(100, '\n');
// }


// Salvarea datelor în fișier
void saveData() {
    ofstream fout("conturi.txt");
    if (fout.is_open()) {
        for (auto c : conturi) {
            fout << c->id << "\n";
            fout << c->nume << "\n";
            fout << c->sold << "\n";
            fout << c->pas << "\n";
            fout << c->tranzactii.size() << "\n";
            for (auto &tr : c->tranzactii) {
                fout << tr << "\n";
            }
            fout << "------------\n"; // separator între conturi
        }
    }
    fout.close();
}

// Încărcarea datelor din fișier
void loadData() {
    ifstream fin("conturi.txt");
    if (!fin.is_open()) return;
    
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        int id = stoi(line);
        string nume;
        getline(fin, nume);
        getline(fin, line);
        float sold = stof(line);
        string parola;
        getline(fin, parola);
        Cont* c = new Cont(id, nume, sold, parola);
        getline(fin, line);
        int numTranz = stoi(line);
        for (int i = 0; i < numTranz; i++) {
            getline(fin, line);
            c->tranzactii.push_back(line);
        }
        getline(fin, line); // se citește separatorul "###"
        conturi.push_back(c);
    }
    fin.close();
}

// Căutare cont după ID în vector
Cont* cautaCont(int id) {
    for (auto c : conturi) {
        if (c->id == id)
            return c;
    }
    return nullptr;
}

// Funcție pentru înregistrarea unui utilizator nou
void registerUser() {
    int id;
    string nume, parola;
    cout << "Introduceti ID-ul: ";
    cin >> id;
    if (cautaCont(id) != nullptr) {
        cout << "ID-ul exista deja!\n";
        return;
    }
    cout << "Introduceti numele: ";
    cin.ignore();
    getline(cin, nume);
    cout << "Introduceti parola: ";
    getline(cin, parola);
    Cont* c = new Cont(id, nume, 0, parola);
    conturi.push_back(c);
    saveData();
    cout << "Cont creat cu succes!\n";
    system("clear");
}

// Funcție de autentificare
Cont* loginUser() {
    int id;
    string parola;
    cout << "Introduceti ID-ul pentru autentificare: ";
    cin >> id;
    Cont* user = cautaCont(id);
    if (user == nullptr) {
        cout << "Cont inexistent!\n";
        return nullptr;
    }
    
    cout << "Introduceti parola: ";
    cin.ignore();
    getline(cin, parola);
    
    if (user->pas != parola) {
        cout << "Parola incorecta!\n";
        return nullptr;
    }
    return user;
}

// Operație de depunere
void deposit(Cont* user) {
    float suma;
    cout << "Introduceti suma de depus: ";
    cin >> suma;
    user->depuneBani(suma);
    saveData();
    cout << "Suma depusa cu succes!\n";
    system("clear");
}

// Operație de retragere
void withdraw(Cont* user) {
    cin.ignore();
    string parola;
    cout << "Introduceti parola: ";
    getline (cin, parola);
    if (user->pas != parola) {
        cout << "Parola incorecta!\n";
        return;
    }
    
    float suma;
    cout << "Introduceti suma de retras: ";
    cin >> suma;
    if (user->retrageBani(suma)) {
        saveData();
        cout << "Retragere reusita!\n";
    } else {
        cout << "Fonduri insuficiente!\n";
    }
    system("clear");
}

// Operație de transfer între conturi
void transfer(Cont* user) {
    cin.ignore();
    string parola;
    cout << "Introduceti parola: ";
    getline (cin, parola);
    if (user->pas != parola) {
        cout << "Parola incorecta!\n";
        return;
    }
    
    int idDest;
    float suma;
    cout << "Introduceti ID-ul destinatarului: ";
    cin >> idDest;
    Cont* destinatie = cautaCont(idDest);
    if (destinatie == nullptr) {
        cout << "Cont destinatar inexistent!\n";
        return;
    }
    cout << "Introduceti suma de transferat: ";
    cin >> suma;
    if (user->trimiteBani(destinatie, suma)) {
        saveData();
        cout << "Transfer efectuat cu succes!\n";
    } else {
        cout << "Fonduri insuficiente pentru transfer!\n";
    }
    system("clear");
}

// Operație de verificare a soldului
void viewBalance(Cont* user) {
    cout << "Soldul curent: " << user->sold << "\n";
    cout << "Apasati Enter pentru a continua...";
    cin.ignore();
    cin.get();
    system("clear");
}

// Operație de vizualizare a tranzacțiilor
void viewTransactions(Cont* user) {
    user->vizualizeazaTranzactii();
    cout << "Apasati Enter pentru a continua...";
    cin.ignore();
    cin.get();
    system("clear");
}

// Operație de blocare (ștergere) a contului
void blockAccount(Cont* user) {
    cin.ignore();
    string parola;
    cout << "Introduceti parola: ";
    getline (cin, parola);
    if (user->pas != parola) {
        cout << "Parola incorecta!\n";
        return;
    }
    string confirmare;

    cout << "Esti precis? da ori nu: ";
    getline (cin, confirmare);
    
    if (confirmare != "da") {
    cout << "Ladna anulare\n";
    cout << "Apasati Enter pentru a continua..";
    cin.get();
    system("clear");
 
    }
    
    int id = user->id;
    user->blocheazaCont();
    // Elimină contul din vector
    for (auto it = conturi.begin(); it != conturi.end(); ++it) {
        if ((*it)->id == id) {
            delete *it;
            conturi.erase(it);
            break;
        }
    }
    saveData();
    cout << "Cont blocat si sters!\n";
    system("clear");
}

int main() {
    loadData();
    int opt;
    while (true) {
        cout << "\n=== ATM System ===\n";
        cout << "1. Inregistrare\n";
        cout << "2. Autentificare\n";
        cout << "3. Iesire\n";
        cout << "Alegeti optiunea: ";
        cin >> opt;
        if (opt == 1) {
            registerUser();
        } else if (opt == 2) {
            Cont* user = loginUser();
            if (user == nullptr) continue;
            int optUser;
            while (true) {
                cout << "\n--- Meniu Utilizator ---\n";
                cout << "1. Depunere bani\n";
                cout << "2. Retragere bani\n";
                cout << "3. Verificare sold\n";
                cout << "4. Transfer bani\n";
                cout << "5. Vizualizare tranzactii\n";
                cout << "6. Blocare cont\n";
                cout << "7. Logout\n";
                cout << "Alegeti optiunea: ";
                cin >> optUser;
                if (optUser == 1) {
                    deposit(user);
                } else if (optUser == 2) {
                    withdraw(user);
                } else if (optUser == 3) {
                    viewBalance(user);
                } else if (optUser == 4) {
                    transfer(user);
                } else if (optUser == 5) {
                    viewTransactions(user);
                } else if (optUser == 6) {
                    blockAccount(user);
                    break;
                } else if (optUser == 7) {
                    break;
                } else {
                    cout << "Optiune invalida!\n";
                    system("clear");
                }
            }
        } else if (opt == 3) {
            break;
        } else {
            cout << "Optiune invalida!\n";
                system("clear");
        }
    }
    
    // Eliberare memorie
    for (auto c : conturi)
        delete c;
    return 0;
}
