#ifndef CSV_H
#define CSV_H

#include <fstream>

using namespace std;

class CSV {
public:
    void initialize(ifstream&);             // Réinitialise le curseur de lecture d'un .csv
    void reader(string);                    // Affiche ligne par ligne le contenu d'un .csv
    string createCSVConfig(string**, int);  // convertis un tableau en .csv

    int getNbPoste(string);                 // Récupère le numéro de poste dans le nom NetBios
    int getNbLignes(ifstream&);             // Récupère le nombre de ligne d'un .csv
    int getNbColonnes(ifstream&);           //  "   "   "   "   "    colonnes d'un .csv

    int* detectRegex(string);               // Lis un .csv et trouve les colonnes du nom NetBios, // addresses MAC et IP

    string getDlPath();
    string genFilename();

    string** sortTab(string**, int);        // Vérifie que le tableau du scan est ordonné dans l'ordre croissant
    string** ordRegex(int*, string**, int);      // Rempli le tableau de config avec le regex
    string** CSVtoTab(string);              // Convertis un .csv en un tableau
};

#endif // CSV_H
