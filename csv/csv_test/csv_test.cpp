#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <regex>

#include "csv_test.h"

using namespace std;

// - Réinitialise le curseur de lecture du fichier
void CSVTest::initialize(ifstream& fichier){
    fichier.clear();            // Efface les indicateurs d'erreur
    fichier.seekg(0, ios::beg); // Retour au début du fichier
}


// - Récupère le numéro de poste du nom NetBios
int CSVTest::getNbPoste(string str){
    string numStr;

    for (size_t i = str.length() - 2; i < str.length(); ++i)    // Récupère les deux derniers caractères de la str spécifiée
        numStr += str[i];

    int numInt = stoi(numStr);                                  // Converti la str en int
    return numInt;
}


// - Range dans l'ordre chaque poste enregistré
string** CSVTest::sortTab(SimTest::TableauA& tableauA) {

    int len = sizeof(tableauA) / sizeof(tableauA[0]);       // Calcule la longueur du tableau
    bool sorted = false;
    int nb = 0;

    string** sortedTabA = new string*[len];                 // Tableau pour stocker les valeurs triées

    for (int i = 0; i < len; ++i)
        sortedTabA[i] = new string[3] {tableauA[i][0], tableauA[i][1], tableauA[i][2]};  // Copie tableauA dans sortedTabA pour le tri

while (sorted==false) {
    for (int i = 0; i < len - 1; ++i) {                     // Parcours tout le tableau
        int posteI = getNbPoste(sortedTabA[i][0]);          // Récupère le numéro de poste i et i+1
        int posteII = getNbPoste(sortedTabA[i+1][0]);       //x

        if (posteI > posteII) {                             // Si le n° de poste i est plus grand que le n° poste i+1
            // Echange de leur place dans le tableau
            string tempLine0 = sortedTabA[i][0];
            string tempLine1 = sortedTabA[i][1];
            string tempLine2 = sortedTabA[i][2];

            sortedTabA[i][0] = sortedTabA[i+1][0];
            sortedTabA[i][1] = sortedTabA[i+1][1];
            sortedTabA[i][2] = sortedTabA[i+1][2];

            sortedTabA[i+1][0] = tempLine0;
            sortedTabA[i+1][1] = tempLine1;
            sortedTabA[i+1][2] = tempLine2;

            nb++;                                           // Incrémente de 1 le nombre de changements
        }
    }
    if (nb == 0)                                            // Si le nombre de changement est nul, le tri s'arrête
        sorted = true;
    else
        nb = 0;                                             // Réinitialisation du nombre de changement à chaque tour
}
return sortedTabA;
}


// - Sauvegarde le tableau dans un fichier .csv
void CSVTest::createCSVConfig(string** tableau) {
    ofstream file("../fichier_test.csv");                       // Créé un fichier .csv dans l'emplacement spécifié

    for (int i = 0; i < 15; ++i) {                              // Inscris les données du tableau dans le fichier .csv
        for (int j = 0; j < 3; ++j) {
            file << tableau[i][j];
            if (j < 2) {
                file << ";";                                    // Ajoute le séparateur (changement de case)
            }
        }
        file << endl;                                           // Saut de ligne à la fin de chaque ligne
    }

    cout << "Sauvegarde : 200" << endl;
    file.close();
}


// Calcule le nombre de lignes du .csv
int CSVTest::getNbLignes(ifstream& fichier){
    initialize(fichier);

    int lignes = 0;
    string ligne;

    while (getline(fichier, ligne))
        lignes++;

    return lignes;
}

// Calcule le nombre de colonnes du .csv
int CSVTest::getNbColonnes(ifstream& fichier){
    initialize(fichier);

    int lignes = 0;
    int colonnes = 0;
    string ligne;

    while (getline(fichier, ligne)){
        int temp_colonnes = 0;

        for (char lettre : ligne)
            if (lettre == ';')
                temp_colonnes++;

        if (temp_colonnes >= colonnes){
            lignes++;
            colonnes = temp_colonnes;
        }
    }
    colonnes++;
    return colonnes;
}

// Lit un fichier .csv externe et récupère les colonnes avec les nom NetBios, addresses MAC et IP || EN COURS
string** CSVTest::readCSV(string chemin){

    ifstream fichier(chemin);

    string ligne;
    string temp_case;

    int len = getNbLignes(fichier);             // Récupère la longueur du .csv
    initialize(fichier);                        // Réinitialise le curseur de lecture

    int lar = getNbColonnes(fichier);           // Récupère la largeur du .csv
    initialize(fichier);                        // Réinitialise le curseur de lecture

    string** newTab = new string*[len];         // Déclare un tableau dynamique de longueur égale au .csv
    for(int i=0; i<len; i++)                    // Déclare dynamiquement le nombre de colonnes
        newTab[i] = new string [lar];

    for(int i=0; i<len; i++){
        getline(fichier, ligne);
        int size = ligne.size();
        int n = 0; // Compteur pour la position dans la ligne

        for(int j=0; j<lar; j++) {
            temp_case = "";

            // Lecture de chaque caractère de la ligne jusqu'au prochain ';'
            while(n < size && ligne[n] != ';'){
                temp_case += ligne[n];
                n++;
            }
            // Ajout de la case dans le tableau
            newTab[i][j] = temp_case;

            // Passer au prochain caractère après le ';'
            n++;
        }
    }

    string nNetBios;
    string addrIP;
    string addrMAC;

    // Définition des expressions régulières
    regex nNetBiosRegex("[A-E]{1}[0-9]{3}-P[0-9]{2}");
    regex addrIPRegex("^([0-9A-Fa-f]{2}[:]){5}([0-9A-Fa-f]{2})$");
    regex addrMACRegex("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    string line;
    while (getline(fichier, line)) {
        istringstream iss(line);
        string column;
        vector<string> filtered_columns;

        while (getline(iss, column, ',')) {
            if (regex_match(column, addrMACRegex) ||
                regex_match(column, addrIPRegex) ||
                regex_match(column, nNetBiosRegex)) {
                filtered_columns.push_back(column);
            }
        }

        // Print the filtered columns
        for (const string& col : filtered_columns) {
            cout << col << ",";
        }
        cout << endl;
    }

    fichier.close();

    return newTab;
}


// Lis le fichier .csv
void CSVTest::reader(string chemin){
    string ligne;

    ifstream fichier(chemin); // - Ouvre le fichier csv spécifié

    if (fichier.is_open()){
        while (getline(fichier, ligne))
            cout << ligne << endl;

        fichier.close();
    }
    else
        cerr << "Erreur lors de l'ouverture du fichier." << endl;
}

