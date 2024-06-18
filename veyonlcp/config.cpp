#include <iostream>
#include <ctime>
#include <fstream> // Pour la lecture de fichiers
#include <sstream> // Pour le traitement des chaînes

#include "config.h"

using namespace std;

Config::Config()
{

}

void Config::deleteConfigVeyon() {
    string veyonCommandName = "networkobjects";
    string veyonArguments = "clear";

    string command = "\"" + string("\"C:/Program Files/Veyon/veyon-cli.exe\"\" ") + veyonCommandName + " " + veyonArguments;

    // cout<< command.c_str() << endl;

    // Ouvrir un pipe pour lire la sortie de la commande
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
      cerr << "Erreur lors de l'ouverture du pipe" << endl;
      return;
    }

    // Lire la sortie de la commande ligne par ligne
    string line;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe)) {
      line += buffer;
    }

    // Fermer le pipe et libérer les ressources
    pclose(pipe);

    // Supprimer les caractères de fin de ligne
    line.erase(line.find_last_not_of("\n\r"));

    // Afficher la sortie de la commande
    cout << "[CONFIG] " << line << endl;
    cout << "[CONFIG] Configuration actuelle supprimee" << endl;
}


// Méthode pour configurer Veyon Master à partir d'un fichier CSV
void Config::configVeyon(string fileName, string salle) {

    deleteConfigVeyon();

    string veyonCommandName = "networkobjects";
    string veyonArguments = "import";
    string csvFormat = "\"%name%;%mac%;%host%\"";

    string command = "\"" + string("\"C:/Program Files/Veyon/veyon-cli.exe\" ") + veyonCommandName + " " + veyonArguments + " " + fileName + " " + "location" + " " + salle +  " " + "format" + " " + csvFormat + "\"";

    // cout<< command.c_str() << endl;

    // Ouvrir un pipe pour lire la sortie de la commande
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
      cerr << "Erreur lors de l'ouverture du pipe" << endl;
      return;
    }

    // Lire la sortie de la commande ligne par ligne
    string line;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe)) {
      line += buffer;
    }

    // Fermer le pipe et libérer les ressources
    pclose(pipe);

    // Supprimer les caractères de fin de ligne
    line.erase(line.find_last_not_of("\n\r"));

    // Afficher la sortie de la commande
    cout << "[CONFIG] " << line << endl;

}
