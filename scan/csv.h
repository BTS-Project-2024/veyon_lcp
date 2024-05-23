#ifndef CSV_H
#define CSV_H

#include <fstream>

using namespace std;

class CSV {
public:
    void initialize(ifstream&);             // Réinitialise le curseur de lecture d'un .csv
    void reader(string);                    // Affiche ligne par ligne le contenu d'un .csv
    void createCSVConfig(string**);         // convertis un tableau en .csv

    int getNbPoste(string);                 // Récupère le numéro de poste dans le nom NetBios
    int getNbLignes(ifstream&);             // Récupère le nombre de ligne d'un .csv
    int getNbColonnes(ifstream&);           //  "   "   "   "   "    colonnes d'un .csv
    int getLenght(string**);                // Renvoie la longueur d'un tableau de pointeur

    int* detectRegex(string);               // Lis un .csv et trouve les colonnes du nom NetBios,
                                            // addresses MAC et IP

    string getDlPath();
    string genFilename();

    string** sortTab(string**);             // Vérifie que le tableau du scan est ordonné
    string** ordRegex(int*, string**);      // Rempli le tableau de config avec le regex
    string** CSVtoTab(string);              // Convertis un .csv en un tableau

};

#endif // CSV_H
