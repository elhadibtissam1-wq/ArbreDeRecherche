
#include <iostream>
#include <map>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;

// 1. METHODE AVEC MAP

void insererMap(map<int, int>& abr, int x) {
    abr.insert({x, x});
}

bool rechercherMap(map<int, int>& abr, int x) {
    return abr.find(x) != abr.end();
}

void supprimerMap(map<int, int>& abr, int x) {
    abr.erase(x);
}

void afficherMap(const map<int, int>& abr) {
    for (const auto& p : abr)
        cout << p.first << " ";
    cout << endl;
}

// 2. METHODE CLASSIQUE : ABR

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droite;

    Noeud(int x) {
        valeur = x;
        gauche = nullptr;
        droite = nullptr;
    }
};

Noeud* insererABR(Noeud* racine, int x) {
    if (racine == nullptr)
        return new Noeud(x);

    if (x < racine->valeur)
        racine->gauche = insererABR(racine->gauche, x);
    else if (x > racine->valeur)
        racine->droite = insererABR(racine->droite, x);

    return racine;
}

bool rechercherABR(Noeud* racine, int x) {
    if (racine == nullptr)
        return false;

    if (x == racine->valeur)
        return true;

    if (x < racine->valeur)
        return rechercherABR(racine->gauche, x);

    return rechercherABR(racine->droite, x);
}

Noeud* minimum(Noeud* racine) {
    while (racine->gauche != nullptr)
        racine = racine->gauche;

    return racine;
}

Noeud* supprimerABR(Noeud* racine, int x) {
    if (racine == nullptr)
        return nullptr;

    if (x < racine->valeur) {
        racine->gauche = supprimerABR(racine->gauche, x);
    }
    else if (x > racine->valeur) {
        racine->droite = supprimerABR(racine->droite, x);
    }
    else {
        if (racine->gauche == nullptr) {
            Noeud* temp = racine->droite;
            delete racine;
            return temp;
        }

        if (racine->droite == nullptr) {
            Noeud* temp = racine->gauche;
            delete racine;
            return temp;
        }

        Noeud* temp = minimum(racine->droite);
        racine->valeur = temp->valeur;
        racine->droite =
            supprimerABR(racine->droite, temp->valeur);
    }

    return racine;
}

void afficherABR(Noeud* racine) {
    if (racine != nullptr) {
        afficherABR(racine->gauche);
        cout << racine->valeur << " ";
        afficherABR(racine->droite);
    }
}

void libererABR(Noeud* racine) {
    if (racine != nullptr) {
        libererABR(racine->gauche);
        libererABR(racine->droite);
        delete racine;
    }
}

// 3. STRUCTURE DES RESULTATS

struct Resultats {
    double insertionMap = 0;
    double insertionABR = 0;
    double rechercheMap = 0;
    double rechercheABR = 0;
    double suppressionMap = 0;
    double suppressionABR = 0;
};

// 4. COMPARAISON DES DEUX METHODES

Resultats tester(vector<int> valeurs, string titre) {
    map<int, int> abrMap;
    Noeud* racine = nullptr;
    Resultats r;

    cout << "\n" << titre << endl;

    cout << "Valeurs saisies : ";
    for (int x : valeurs)
        cout << x << " ";
    cout << endl;

    auto debut = steady_clock::now();

    for (int x : valeurs)
        insererMap(abrMap, x);

    auto fin = steady_clock::now();

    r.insertionMap =
        duration<double, micro>(fin - debut).count();

    debut = steady_clock::now();

    for (int x : valeurs)
        racine = insererABR(racine, x);

    fin = steady_clock::now();

    r.insertionABR =
        duration<double, micro>(fin - debut).count();

    cout << "\nAffichage des arbres\n";
    cout << "MAP : ";
    afficherMap(abrMap);

    cout << "ABR classique : ";
    afficherABR(racine);
    cout << endl;

    cout << "\nTemps d'insertion\n";
    cout << "MAP : " << r.insertionMap << " microsecondes\n";
    cout << "ABR classique : " << r.insertionABR
         << " microsecondes\n";

    int x;
    cout << "\nValeur a rechercher : ";
    cin >> x;

    debut = steady_clock::now();
    bool trouveMap = rechercherMap(abrMap, x);
    fin = steady_clock::now();

    r.rechercheMap =
        duration<double, micro>(fin - debut).count();

    debut = steady_clock::now();
    bool trouveABR = rechercherABR(racine, x);
    fin = steady_clock::now();

    r.rechercheABR =
        duration<double, micro>(fin - debut).count();

    cout << "\nTemps de recherche\n";
    cout << "MAP : "
         << (trouveMap ? "Valeur trouvee" : "Valeur absente")
         << " ; " << r.rechercheMap << " microsecondes\n";

    cout << "ABR classique : "
         << (trouveABR ? "Valeur trouvee" : "Valeur absente")
         << " ; " << r.rechercheABR << " microsecondes\n";

    cout << "\nValeur a supprimer : ";
    cin >> x;

    debut = steady_clock::now();
    supprimerMap(abrMap, x);
    fin = steady_clock::now();

    r.suppressionMap =
        duration<double, micro>(fin - debut).count();

    debut = steady_clock::now();
    racine = supprimerABR(racine, x);
    fin = steady_clock::now();

    r.suppressionABR =
        duration<double, micro>(fin - debut).count();

    cout << "\nTemps de suppression\n";
    cout << "MAP : " << r.suppressionMap << " microsecondes\n";
    cout << "ABR classique : " << r.suppressionABR
         << " microsecondes\n";

    cout << "\nArbres apres suppression\n";
    cout << "MAP : ";
    afficherMap(abrMap);

    cout << "ABR classique : ";
    afficherABR(racine);
    cout << endl;

    libererABR(racine);

    return r;
}

// 5. AFFICHAGE DU TABLEAU COMPARATIF


void afficherTableau(Resultats aleatoire, Resultats croissant) {
    cout << "\n\nTABLEAU COMPARATIF DES TEMPS D'EXECUTION\n\n";

    cout << left
         << setw(16) << "Operation"
         << setw(22) << "Methode"
         << setw(25) << "Arbre aleatoire"
         << setw(25) << "Arbre croissant"
         << endl;

    cout << string(88, '-') << endl;
    cout << fixed << setprecision(3);

    cout << left << setw(16) << "Insertion"
         << setw(22) << "MAP"
         << setw(25) << aleatoire.insertionMap
         << setw(25) << croissant.insertionMap << endl;

    cout << left << setw(16) << "Insertion"
         << setw(22) << "ABR classique"
         << setw(25) << aleatoire.insertionABR
         << setw(25) << croissant.insertionABR << endl;

    cout << left << setw(16) << "Recherche"
         << setw(22) << "MAP"
         << setw(25) << aleatoire.rechercheMap
         << setw(25) << croissant.rechercheMap << endl;

    cout << left << setw(16) << "Recherche"
         << setw(22) << "ABR classique"
         << setw(25) << aleatoire.rechercheABR
         << setw(25) << croissant.rechercheABR << endl;

    cout << left << setw(16) << "Suppression"
         << setw(22) << "MAP"
         << setw(25) << aleatoire.suppressionMap
         << setw(25) << croissant.suppressionMap << endl;

    cout << left << setw(16) << "Suppression"
         << setw(22) << "ABR classique"
         << setw(25) << aleatoire.suppressionABR
         << setw(25) << croissant.suppressionABR << endl;

    cout << "\nTemps exprimes en microsecondes (us).\n";
}


// 6. PROGRAMME PRINCIPAL

int main() {
    int n;

    cout << "Nombre de valeurs pour chaque test : ";
    cin >> n;

    if (n <= 0) {
        cout << "Nombre invalide.\n";
        return 1;
    }

    vector<int> valeursAleatoires(n);
    vector<int> valeursCroissantes(n);

    cout << "\nTEST 1 : entrez les valeurs dans un ordre aleatoire.\n";

    for (int i = 0; i < n; i++) {
        cout << "Valeur " << i + 1 << " : ";
        cin >> valeursAleatoires[i];
    }

    cout << "\nTEST 2 : entrez les valeurs en ordre croissant.\n";

    for (int i = 0; i < n; i++) {
        cout << "Valeur " << i + 1 << " : ";
        cin >> valeursCroissantes[i];
    }

    Resultats rAleatoire =
        tester(valeursAleatoires, "TEST 1 : ORDRE ALEATOIRE");

    Resultats rCroissant =
        tester(valeursCroissantes, "TEST 2 : ORDRE CROISSANT");

    afficherTableau(rAleatoire, rCroissant);

    return 0;
}
