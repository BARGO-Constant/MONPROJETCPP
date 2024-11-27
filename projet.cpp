#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <ctime>
using namespace std;

using namespace std;

void effacerEcran() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nAppuyez sur Entree pour continuer...";
    cin.ignore();
    cin.get();
}

#include <windows.h>

void attendre(int millisecondes) {
    Sleep(millisecondes);
}



void afficherMenu() {
    cout << "\n=== SIMULATEUR BANCAIRE ===\n";
    cout << "1. Creer un compte\n";
    cout << "2. Effectuer un depot\n";
    cout << "3. Effectuer un retrait\n";
    cout << "4. Consulter un compte\n";
    cout << "5. Effectuer un virement\n";
    cout << "6. Verrouiller le compte\n";
    cout << "7. Deverrouiller le compte\n";
    cout << "8. Changer le mot de passe\n";
    cout << "9. Quitter\n";
    cout << "Votre choix: ";
}

class Transaction {
public:
    string type;
    double montant;
    string horodatage;
    
    Transaction(string t, double m) : type(t), montant(m) {
        time_t maintenant = time(0);
        horodatage = ctime(&maintenant);
    }
};

class Compte {
    string numero;
    string nom;
    double solde;
    string motDePasse;
    bool verrouille;
    vector<Transaction> transactions;

public:
    Compte(string num = "", string n = "", double s = 0.0, string mdp = "") : 
        numero(num), nom(n), solde(s), motDePasse(mdp), verrouille(false) {}

    bool verifierMotDePasse(const string& mdp) const {
        return motDePasse == mdp;
    }

    bool estVerrouille() const {
        return verrouille;
    }

    void definirVerrouillage(bool statut) {
        verrouille = statut;
    }

    bool changerMotDePasse(const string& ancienMdp, const string& nouveauMdp) {
        if (motDePasse == ancienMdp) {
            motDePasse = nouveauMdp;
            return true;
        }
        return false;
    }

    bool deposer(double montant) {
        if (!verrouille && montant > 0) {
            solde += montant;
            transactions.push_back(Transaction("Depot", montant));
            return true;
        }
        return false;
    }

    bool retirer(double montant, const string& mdp) {
        if (verrouille || !verifierMotDePasse(mdp) || montant <= 0) {
            return false;
        }
        
        if (solde >= montant) {
            solde -= montant;
            transactions.push_back(Transaction("Retrait", -montant));
            return true;
        }
        return false;
    }

    void afficher() {
        cout << "\nCompte N°" << numero << "\n";
        cout << "Client: " << nom << "\n";
        cout << fixed << setprecision(2);
        cout << "Solde: " << solde << "f\n";
        cout << "Statut: " << (verrouille ? "Verrouille" : "Actif") << "\n";
        cout << "\nTransactions:\n";
        for (const auto& t : transactions) {
            cout << t.horodatage.substr(0, t.horodatage.length()-1) << " : ";
            cout << t.type << " " << abs(t.montant) << "f\n";
        }
    }

    string getNumero() { return numero; }
    string getNom() { return nom; }
    double getSolde() { return solde; }
};

class Banque {
    map<string, Compte> comptes;

public:

   
    
    void creerCompte() {
    string num, nom, mdp;
    double solde;
    bool numeroValide;
    
    effacerEcran();
    
    do {
        numeroValide = true;
        cout << "Numero de compte: ";
        cin >> num;
        
        // Vérifier que le numéro contient uniquement des chiffres positifs
        for (char c : num) {
            if (!isdigit(c)) {
                numeroValide = false;
                break;
            }
        }
        
        if (!numeroValide) {
            cout << "Le numero de compte doit contenir uniquement des chiffres!\n";
            continue;
        }
        
        if (num[0] == '0') {
            cout << "Le numero de compte ne peut pas commencer par 0!\n";
            numeroValide = false;
            continue;
        }
        
        if (comptes.find(num) != comptes.end()) {
            cout << "Ce numero de compte existe deja!\n";
            numeroValide = false;
        }
        
    } while (!numeroValide);
    
    cout << "Nom du client: ";
    cin.ignore();
    getline(cin, nom);
    
    do {
        cout << "Solde initial: ";
        cin >> solde;
        if (solde <= 0) {
            cout << "Le solde initial doit etre positif!\n";
        }
    } while (solde <= 0);
    
    cout << "Creer un mot de passe: ";
    cin >> mdp;
    
    comptes[num] = Compte(num, nom, solde, mdp);
    cout << "\nCompte cree avec succes!\n";
    attendre(1000);
}



    void deposer() {
        string num;
        double montant;
        
        effacerEcran();
        cout << "Numero de compte: ";
        cin >> num;
        
        if (comptes.find(num) != comptes.end()) {
        if (comptes[num].estVerrouille()) {
            cout << "\nLe compte est verrouillé!\n";
            attendre(1000);
            return;
        }
        cout << "Montant a deposer: ";
        cin >> montant;
        if (montant <= 0) {  
            cout << "\nLe montant doit etre positif!\n";
        }
        else if (comptes[num].deposer(montant)) {
            cout << "\nDepot effectue avec succes!\n";
        }
    } else {
        cout << "\nCompte non trouve!\n";
    }
    attendre(1000);
}

        void retirer() {
        string num, mdp;
        double montant;
        
        effacerEcran();
        cout << "Numero de compte: ";
        cin >> num;
        
        if (comptes.find(num) != comptes.end()) {
            if (comptes[num].estVerrouille()) {
                cout << "\nLe compte est verrouillé!\n";
                attendre(1000);
                return;
            }
            cout << "Mot de passe: ";
            cin >> mdp;
            cout << "Montant a retirer: ";
            cin >> montant;
            
            if (montant <= 0) { 
            cout << "\nLe montant doit etre positif!\n";
        }
        else if (comptes[num].retirer(montant, mdp)) {
            cout << "\nRetrait effectue avec succes!\n";
        } else {
            cout << "\nMot de passe incorrect ou fonds insuffisants!\n";
        }
    } else {
        cout << "\nCompte non trouve!\n";
    }
    attendre(1000);
}

    void afficherCompte() {
        string num, mdp;
        effacerEcran();
        cout << "Numéro de compte: ";
        cin >> num;
        cout << "Mot de passe: ";
        cin >> mdp;
        
        if (comptes.find(num) != comptes.end()) {
            if (comptes[num].verifierMotDePasse(mdp)) {
                comptes[num].afficher();
            } else {
                cout << "\nMot de passe incorrect!\n";
            }
        } else {
            cout << "\nCompte non trouve!\n";
        }
        pause();
    }

    void virement() {
        string source, dest, mdp;
        double montant;
        
        effacerEcran();
        cout << "Compte source: ";
        cin >> source;
        cout << "Mot de passe: ";
        cin >> mdp;
        cout << "Compte destination: ";
        cin >> dest;
        
        if (comptes.find(source) != comptes.end() && 
        comptes.find(dest) != comptes.end()) {
        if (comptes[source].estVerrouille() || comptes[dest].estVerrouille()) {
            cout << "\nL'un des comptes est verrouille!\n";
            attendre(1000);
            return;
        }
        cout << "Montant: ";
        cin >> montant;
        
        if (montant <= 0) {  
            cout << "\nLe montant doit etre positif!\n";
        }
        else if (comptes[source].retirer(montant, mdp)) {
            comptes[dest].deposer(montant);
            cout << "\nVirement effectue avec succes!\n";
        } else {
            cout << "\nMot de passe incorrect ou fonds insuffisants!\n";
        }
    } else {
        cout << "\nL'un des comptes n'existe pas!\n";
    }
    attendre(1000);
}


    void verrouillerCompte() {
        string num, mdp;
        effacerEcran();
        cout << "Numero de compte: ";
        cin >> num;
        cout << "Mot de passe: ";
        cin >> mdp;
        
        if (comptes.find(num) != comptes.end()) {
            if (comptes[num].verifierMotDePasse(mdp)) {
                comptes[num].definirVerrouillage(true);
                cout << "\nCompte verrouille avec succes!\n";
            } else {
                cout << "\nMot de passe incorrect!\n";
            }
        } else {
            cout << "\nCompte non trouve!\n";
        }
        attendre(1000);
    }

    void deverrouillerCompte() {
        string num, mdp;
        effacerEcran();
        cout << "Numero de compte: ";
        cin >> num;
        cout << "Mot de passe: ";
        cin >> mdp;
        
        if (comptes.find(num) != comptes.end()) {
            if (comptes[num].verifierMotDePasse(mdp)) {
                comptes[num].definirVerrouillage(false);
                cout << "\nCompte déverrouillé avec succès!\n";
            } else {
                cout << "\nMot de passe incorrect!\n";
            }
        } else {
            cout << "\nCompte non trouvé!\n";
        }
        attendre(1000);
    }

    void changerMotDePasse() {
        string num, ancienMdp, nouveauMdp;
        effacerEcran();
        cout << "Numero de compte: ";
        cin >> num;
        cout << "Ancien mot de passe: ";
        cin >> ancienMdp;
        cout << "Nouveau mot de passe: ";
        cin >> nouveauMdp;
        
        if (comptes.find(num) != comptes.end()) {
            if (comptes[num].changerMotDePasse(ancienMdp, nouveauMdp)) {
                cout << "\nMot de passe change avec succes!\n";
            } else {
                cout << "\nMot de passe incorrect!\n";
            }
        } else {
            cout << "\nCompte non trouve!\n";
        }
        attendre(1000);
    }
};

int main() {
    Banque banque;
    int choix;
    
    while (true) {
        effacerEcran();
        afficherMenu();
        cin >> choix;
        
        switch (choix) {
            case 1: banque.creerCompte(); break;
            case 2: banque.deposer(); break;
            case 3: banque.retirer(); break;
            case 4: banque.afficherCompte(); break;
            case 5: banque.virement(); break;
            case 6: banque.verrouillerCompte(); break;
            case 7: banque.deverrouillerCompte(); break;
            case 8: banque.changerMotDePasse(); break;
            case 9: 
                cout << "\nMerci d'avoir utilise nos services!\n";
                attendre(1000);
                return 0;
            default: 
                cout << "\nOption invalide!\n";
                attendre(1000);
        }
    }
    return 0;
}

