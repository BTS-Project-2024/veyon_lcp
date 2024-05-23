#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <regex>
#include <iomanip>

#include "csv.h"

using namespace std;

// - Réinitialise le curseur de lecture du fichier
void CSV::initialize(ifstream& fichier){
    fichier.clear();            // Efface les indicateurs d'erreur
    fichier.seekg(0, ios::beg); // Retour au début du fichier
}

// Récupère l'emplacement du dossier de téléchargement
string CSV::getDlPath(){
    const char* downloadPath = getenv("USERPROFILE");
    return string(downloadPath) + "\\Downloads\\";
}

// Génère le nom du fichier .csv au format 'CSVConfig.LCP_jj-mm_hh.mm'
string CSV::genFilename(){
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    stringstream ss;
    ss << "CSVConfigLCP." << setw(2) << setfill('0') << localTime->tm_mday << "-"
       << setw(2) << setfill('0') << localTime->tm_mon + 1 << "_"
       << setw(2) << setfill('0') << localTime->tm_hour << "."
       << setw(2) << setfill('0') << localTime->tm_min;

    return ss.str();
}

// - Récupère le numéro de poste du nom NetBios
int CSV::getNbPoste(string str){
    string numStr;

    for (size_t i = str.length() - 2; i < str.length(); ++i)    // Récupère les deux derniers caractères de la str spécifiée
        numStr += str[i];

    int numInt = stoi(numStr);                                  // Converti la str en int
    return numInt;
}


// Calcule le nombre de lignes du .csv
int CSV::getNbLignes(ifstream& fichier){
    initialize(fichier);

    int lignes = 0;
    string ligne;

    while (getline(fichier, ligne))
        lignes++;

    return lignes;
}


// Calcule le nombre de colonnes du .csv
int CSV::getNbColonnes(ifstream& fichier){

    initialize(fichier);

    int lignes = 0;
    int colonnes = 0;
    string ligne;

    while (getline(fichier, ligne)){
        int temp_colonnes = 0;

        for (char lettre : ligne)           // Parcours chaque caractère de la str "ligne"
            if (lettre == ';')
                temp_colonnes++;

        if (temp_colonnes >= colonnes){     // Si une ligne contient moins de case qu'une autre
            lignes++;                       // le nombre de colonne est égal au plus grand nombre de case
            colonnes = temp_colonnes;
        }
    }
    colonnes++;
    return colonnes;
}


// Calcule la longueur d'un tableau de pointeur
int CSV::getLenght(string** tab){
    int size = 0;                                   // Calcule la taille du tableau
    while (tab[size] != nullptr)                    // Incrémente 'size' jusqu'à arriver au pointeur nul
        size++;

    return size;
}


// Lit un fichier .csv externe et le stocke dans un tableau
string** CSV::CSVtoTab(string chemin){
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
    fichier.close();
    return newTab;
}


// Récupère les colonnes dans lesquelles sont stockée le nom NetBios, addresse MAC et IP
int* CSV::detectRegex(string chemin){

    ifstream csvFile(chemin);

    regex netbios_regex("[A-E]{1}[0-9]{3}-P[0-9]{2}");
    regex ip_regex("^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?).(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    regex mac_regex("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");

    string line;
    int lineCount = 0;
    int* regexTab = new int[3];

    while (getline(csvFile, line)) {                        // Analyse chaque ligne du .csv
        lineCount++;
        string delimiter;
        if (getline(csvFile, delimiter, ';')) {
            int col = 0;
            stringstream ss(line);
            string value;

            while (getline(ss, value, ';')) {               // Pour chaque ligne du .csv, vérifie la compatibilité entre chaque case et les regex
                col++;

                if (regex_match(value, netbios_regex))
                    regexTab[0] = col;

                else if (regex_match(value, mac_regex))
                    regexTab[1] = col;

                else if (regex_match(value,ip_regex ))
                    regexTab[2] = col;
            }
        }
    }
    return regexTab;
}


// Rempli le tableau de config après lecture du .csv fourni
string** CSV::ordRegex(int* regexTab, string** tabToOrder){

    int size = getLenght(tabToOrder);                       // Calcule la taille du tableau

    string** orderedTab = new string*[size];                // Déclare un tableau de taille len x 3
    for(int i=0; i<size; i++)
        orderedTab[i] = new string [3];

    for(int i=0; i<size; i++)                               // A l'aide de la méthode regexTab,
        for(int j=0; j<3; j++)                              // remplis 'orderedTab' avec les colonnes souhaitées
            orderedTab[i][j] = tabToOrder[i][regexTab[j]];

    return orderedTab;
}

// - Sauvegarde le tableau dans un fichier .csv
void CSV::createCSVConfig(string** tableau) {

    string dl_path = getDlPath();                   // Récupère l'emplacement de téléchargement
    string name = genFilename();                    // Génère le nom du fichier
    string filename = dl_path+name+".csv";

    ofstream file(filename);                        // Créé un fichier .csv dans l'emplacement spécifié

    int size = getLenght(tableau);                  // Récupère la taille du tableau

    for (int i = 0; i < size; ++i) {               // Inscris les données du tableau dans le fichier .csv
        for (int j = 0; j < 3; ++j) {
            file << tableau[i][j];
            if (j < 2)
                file << ";";                        // Ajoute le séparateur (changement de case)
        }
        file << endl;                               // Saut de ligne à la fin de chaque ligne
    }

    cout << "Sauvegarde : 200" << endl;
    file.close();
}


// Lis le fichier .csv
void CSV::reader(string chemin){
    string ligne;

    ifstream fichier(chemin);              // Ouvre le fichier csv spécifié

    if (fichier.is_open())                 // Affiche ligne par ligne
        while (getline(fichier, ligne))
            cout << ligne << endl;

    fichier.close();
}

// - Ordonne un tableau trié
string** CSV::sortTab(string** tableauA) {

    int len = sizeof(tableauA) / sizeof(tableauA[0]);       // Calcule la longueur du tableau
    bool sorted = false;
    int nb = 0;

    string** sortedTabA = new string*[len];                                                // Tableau pour stocker les valeurs triées
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
