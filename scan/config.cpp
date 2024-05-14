#include <iostream>
#include <ctime>
#include <fstream> // Pour la lecture de fichiers
#include <sstream> // Pour le traitement des chaînes

#include "config.h"

using namespace std;

Config::Config()
{

}

string Config::executePowerShellCommand(const char* command){

    string result = "";                                                             // Déclaration d'une chaîne pour stocker la sortie de la commande PowerShell
    FILE* pipe = _popen(command, "r");                                              // Ouverture d'un pipe pour exécuter la commande PowerShell en mode lecture

    if (!pipe) {                                                                    // Vérification si le pipe a été ouvert avec succès
        cerr << "Erreur lors de l'exécution de la commande PowerShell." << endl;    // Affichage d'une erreur si le pipe n'a pas pu être ouvert
        return result;                                                              // Retourne une chaîne vide en cas d'erreur
    }

    char buffer[128];                                                               // Déclaration d'un tampon pour stocker les données lues à partir du pipe

    while (!feof(pipe))                                                             // Lecture des données à partir du pipe jusqu'à la fin du fichier
        if (fgets(buffer, 128, pipe) != NULL)                                       // Lecture d'une ligne du pipe dans le tampon
            result += buffer;                                                       // Concaténation de la ligne lue au résultat

    _pclose(pipe);                                                                  // Fermeture du pipe après la lecture
    return result;                                                                  // Retourne la sortie de la commande PowerShell
}

// Méthode pour configurer Veyon Master à partir d'un fichier CSV
void Config::ConfigVeyon() {

    string commandName = "networkobjects";          // Définition des arguments pour la commande veyon-cli.exe
    string arguments = "import ";                   //
    string location = "'A404' ";                    //
    string format = "'%name%;%mac%;%host%'";        //


    string cheminFichierCSV = getCurrentDateTime(); // Construire le nom du fichier CSV avec la date et l'heure actuelles
    cheminFichierCSV += ".csv";


    ifstream file(cheminFichierCSV);                // Lecture du fichier CSV et configuration de Veyon pour chaque ligne
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);                                                           // Créer un flux de chaîne à partir de la ligne lue
            string name, mac, host;
            if (getline(ss, name, ',') && getline(ss, mac, ',') && getline(ss, host)) {      // Extraire les valeurs de nom, mac et hôte séparées par des virgules
                // Construire la commande PowerShell pour configurer Veyon avec les valeurs extraites
                string powerShellCommand = "powershell -Command \"& 'C:/Program Files/Veyon/veyon-cli.exe' " + commandName + " " + arguments + " '"
                        + cheminFichierCSV + "' location " + location + " format " + format + " name '" + name + "' mac '" + mac + "' host '" + host + "'";
                string output = executePowerShellCommand(powerShellCommand.c_str());          // Exécuter la commande PowerShell et récupérer la sortie
                cout << "Sortie de veyon-cli.exe pour " << name << " :\n" << output << endl;  // Afficher la sortie
            } else
                cerr << "Erreur: Format de ligne incorrect dans le fichier CSV." << endl;     // Afficher une erreur si le format de la ligne dans le fichier CSV est incorrect

        }
        file.close();                                                    // Fermer le fichier CSV après lecture
    } else
        cerr << "Erreur lors de l'ouverture du fichier CSV." << endl;    // Afficher une erreur si le fichier CSV ne peut pas être ouvert
}

// Fonction pour obtenir la date et l'heure actuelles au format jj-mm_hh.mm
string Config::getCurrentDateTime() {

    time_t now = time(0);                        // Obtenir la date et l'heure actuelles
    tm *ltm = localtime(&now);


    string dateTime = "CSVConfigLCP.";           // Construire la chaîne jj-mm_hh.mm
    dateTime += to_string(1 + ltm->tm_mon);      // Mois (janvier = 1)
    dateTime += "-";
    dateTime += to_string(ltm->tm_mday);         // Jour
    dateTime += "_";
    dateTime += to_string(ltm->tm_hour);         // Heure
    dateTime += ".";
    dateTime += to_string(ltm->tm_min);          // Minutes

    return dateTime;
}
