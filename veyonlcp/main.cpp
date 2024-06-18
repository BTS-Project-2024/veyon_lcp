#include <QCoreApplication>
#include <iostream>
#include <thread>
#include <windows.h>
#include <regex>

#include "scan.h"
#include "csv.h"
#include "config.h"

using namespace std;
// -- PROD VERSION -- //
int main() {
    Scan prod_scan;         // Déclare un objet de la classe "scan"
    CSV prod_csv;           // Déclare un objet de classe "CSV"
    Config prod_config;     // Déclare un objet de la classe "Config"

    string salle;
    bool format_salle = false;
    regex salle_regex("[A-M]{1}[0-9]{3}");                                                  // Regex définissant le format du nom d'une salle

    int choix=0;
    bool format_choix = false;

    int regex[3];                                                                           // Déclare le tableau qui accueillera les indices du regex

    string path;

    cout << "\n BIENVENUE DANS VEYON CONFIG LCP" << endl;
    cout << " Enzo DELHAYE WOLFF, Adrien STUCKI et Maxime MORAND \n\n" << endl;

    while (!format_choix) {                                                                 // Force l'utilisateur à choisir entre 1 et 2
        cout << "\n\n Veuillez choisir le mode de configuration : " << endl;
        cout << " 1 : Configuration par SCAN IP" << endl;                                   // En choisisant 1, il lance la configuration par SCAN IP
        cout << " 2 : Configuration par lecture CSV" << endl;                               // En choisisant 2, il lance la configuration en lisant un.csv
        cout << "\n\n Votre choix : ";
        cin >> choix;
        cout << "\n\n\n";

        if (choix == 1 || choix == 2) {                                                     // Si l'utilisateur choisi 1 ou 2, le programme continue
            format_choix = true; // Le choix est valide
        }
        else if (cin.fail()) {                                                              // Si l'utilisateur entre autre chose qu'un int
            cin.clear();                                                                    // Vider le buffer d'entrée
            cin.ignore(numeric_limits<streamsize>::max(), '\n');                            // Ignore les caractères non valides
            cout << " \n Erreur : Saisie invalide. Veuillez selectionner 1 ou 2" << endl;
        }
    }

    if(choix==1){ // Lance la configuration par scan IP
        while(format_salle != true){                                                        // Force l'utilisateur à entrer un nom de salle correct
            cout << "\n Veuillez choisir une salle (ex: A402) : ";
            cin >> salle;

            if (regex_match(salle, salle_regex))                                            // Vérifie si le nom de salle rentré est compatible avec le regex
                format_salle = true;

            else
                cout << "\n Format incorrect \n" << endl;
        }

        prod_scan.setSalle(salle);                                                          // Defini la salle à scanner
        prod_scan.run_tscan();                                                              // Lance le scan
        string** scanTab = prod_scan.getTab();                                              // Récupère le tableau généré

        string** sortedTab = prod_csv.sortTab(scanTab, prod_scan.getTabSize());             // Range le tableau dans l'ordre croissant (lignes)

        string csv_file_path = prod_csv.createCSVConfig(sortedTab, prod_scan.getTabSize()); // Génère le fichier .csv et renvoie sa localisation

        prod_config.configVeyon(csv_file_path, salle);                                      // Fait la configuration de Veyon M
    }

    if(choix==2) { // Lance la configuration par lecture CSV
        while(format_salle != true){                                                        // Force l'utilisateur à entrer un nom de salle correct
            cout << "\n Veuillez choisir une salle (ex: A402) : ";
            cin >> salle;

            if (regex_match(salle, salle_regex))                                            // Vérifie si le nom de salle rentré est compatible avec le regex
                format_salle = true;

            else
                cout << "\n Format incorrect \n" << endl;
        }
        cout << " Veuillez entrer le chemin vers le fichier CSV:" << endl;                 // L'utilisateur doit glisser le fichier .csv dans
        cin >> path;                                                                       // l'invite de commande3


        ifstream file(path);                                                               // Ouvre le fichier spécifié
        string** newTab = prod_csv.CSVtoTab(path);                                         // Rempli un tableau avec le contenu du fichier

        for(int i=0; i<3; i++){                                                            // Rempli ce tableau avec le retour de "detectRegex"
            regex[i] = prod_csv.detectRegex(path)[i];
            cout << regex[i] << " " << endl;
        }

        string** orderedTab = prod_csv.ordRegex(regex, newTab, prod_csv.getNbLignes(file)); // Créé une nouvelle variable pour le tableau rangé dans l'ordre

        string newPath = prod_csv.createCSVConfig(orderedTab, prod_csv.getNbLignes(file));  // Créé un nouveau fichier "propre"
        prod_config.configVeyon(newPath, salle);                                            // Lance la configuration à l'aide du fichier

        file.close();
    }
    system("pause");

    return 0;
}

