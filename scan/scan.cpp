#include "scan.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

scan::scan() {
    // Initialisation du tableau
    list_temp = new string*[50];
    for(int i=0; i<50; i++)
        list_temp[i] = new string[3];

}

void scan::clean_tab() {
    // Permet d'ajouter des cases vides après la liste finie de postes detectés
    for (int i=incTab; i<50 ; i++) {
        for (int j=0; j<3; j++) {
            list_temp[i][j] = "";
        }
    }
}

string scan::getMACOutput(string nomNetbios) {
    // Ouverture d'un pipe "sous-processus" pour lire la sortie de la commande
    string cmd = "nbtstat -a " + nomNetbios;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return "popen failed!";
    }

    string result;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        // Ajout des données lues à la chaîne de caractères
        result += buffer;
    }

    // Fermeture du pipe
    int exitCode = pclose(pipe);

    // Vérification des erreur lors de l'execution de la commande
    if (exitCode != EXIT_SUCCESS) {
        return "Command failed!";
    }

    // Supprime le caractère de fin de ligne (facultatif)
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}

vector<pair<string, string>> scan::getIPOutput() {
    vector<pair<string, string>> arpEntries;

    // Execute la commande "arp -a" et capture sa sortie
    FILE* pipe = _popen("arp -a", "r");
    if (!pipe) {
        cerr << "Error: Failed to execute 'arp -a' command." << endl;
        return arpEntries;
    }

    // On définit un buffer pour la sortie de la commande arp
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string line(buffer);

        // Evite les lignes qui ne contiennent pas de paires IP-MAC
        if (line.find("Interface:") != string::npos || line.find("  ") == string::npos) {
            continue;
        }

        // Extraction des adresses IP et MAC de la ligne
        istringstream iss(line);
        string ip, mac;
        iss >> ip >> mac;

        // Convertit l'adresse MAC en majuscules pour plus de cohérence
        transform(mac.begin(), mac.end(), mac.begin(), ::toupper);
        arpEntries.push_back(make_pair(ip, mac));
    }

    // On ferme le sous processus cmd
    _pclose(pipe);
    return arpEntries;
}


bool scan::scanMACIP(string nomNetbios)
{

    // Commande NBTSTAT avec le nom de l'ordinateur au choix
    string output = getMACOutput(nomNetbios);

    // Récupération de l'adresse MAC parmi le resultat de la commande avec un regex
    string macAddress;
    regex e ("([0-9A-Fa-f]{2}-){5}[0-9A-Fa-f]{2}");

    // Utilisation d'un itérateur prédéfini pour parcourir les résultats
    sregex_iterator begin(output.begin(), output.end(), e);
    sregex_iterator end;

    // Vérification si une adresse MAC a été trouvée
    if (begin != end) {
        // Affichage des correspondances
        for (sregex_iterator i = begin; i != end; ++i) {
            smatch match = *i;
            macAddress = match.str();
        }
    }

    // Récupération de l'adresse IP grâce à l'adresse MAC dans la table ARP (Par commande arp -a)
    // Transforme l'adresse MAC en majuscule pour plus de cohérence
    transform(macAddress.begin(), macAddress.end(), macAddress.begin(), ::toupper);
    vector<pair<string, string>> arpEntries = getIPOutput();

    // Trouve l'adresse IP exact avec l'adresse MAC demandé
    string ipAddress;
    for (const auto& entry : arpEntries) {
        if (entry.second == macAddress) {
            ipAddress = entry.first;
            break;
        }
    }

    std::stringstream transformed_mac;

    // Interroge chaque paire de caractères et ajouter " :" après chaque paire.
    for (unsigned long long i = 0; i < macAddress.length(); i += 3) {
        transformed_mac << macAddress.substr(i, 2);
        if (i < macAddress.length() - 2) {
            transformed_mac << ":";
        }
    }

    // Transformation du char array mac en string
    string smac_address = transformed_mac.str();
    // Réponse finale du résultat de la méthode (Adresse MAC trouvée ou non)
    if (!ipAddress.empty()) {
        // On lock avec le mutex pour ne pas interferer avec d'autres threads
        mtx.lock();
        cout << "Computer: " << nomNetbios << " | MAC Address: " << smac_address << " | IP Address: " << ipAddress << endl;
        // On inscrit les informations dans le tableau puis on incrémente la ligne du tableau pour le thread suivant
        list_temp[incTab][0] = nomNetbios;
        list_temp[incTab][1] = smac_address;
        list_temp[incTab][2] = ipAddress;
        cout << incTab << endl;
        incTab++;
        // On débloque le mutex puis on retourne une réponse booléenne true pour dire que le scan est correct
        mtx.unlock();
        return true;
    } else {
        // Si on a pas d'info, alors, on retourne false
        mtx.lock();
        cout << "No info found about \"" << nomNetbios << "\"" << endl;
        mtx.unlock();
        return false;
    }

}

string** scan::getTab() {
    /* A servi au debug
    for(int a = 0; a < 50; a++)
    {
      for(int b = 0; b < 3; b++)
      {
        cout << list_temp[a][b] << " ";
      }
      cout << endl;
    }
    */
    // On retourne le tableau de la liste des PC scannés
    return this->list_temp;

}

void scan::run_scan(int PC) {
    // Initialisation du nom du poste à scanner
    string nomposte;
    // Formatage du nom du poste selon le numéro du PC
    if (PC < 10) {
        nomposte = salle + "-P" + "0" + to_string(PC);
    } else {
        nomposte = salle + "-P" + to_string(PC);
    }
    // On lance la méthode principale du scan avec le bon nom de poste
    scanMACIP(nomposte);
}

void scan::run_tscan() {
    // Démarrage du Thread Pool
    ThreadScan.Start();
    // Numéro du poste
    int PC = 1;
    // On génére le ThreadPool avec les 99 PC à scanner
    while (PC<51) {
        // cout << PC << endl; A servi au debug
        ThreadScan.QueueJob([this,PC]{run_scan(PC);});
        PC++;
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Attendre un peu avant de revérifier
    }
    // On met le programme en attente tant que le threadpool n'est pas terminé
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // cout << ThreadScan.busy() << endl; A servi au debug
    }
    while (ThreadScan.busy());
    // Scan en multithreading terminé
    ThreadScan.Stop();
    clean_tab();

}
