#ifndef SCAN_H
#define SCAN_H
#include <string>
#include <iostream>
#include <regex>

using namespace std;

class scan
{
private:
    string salle;
    string* list_temp;
    string* list_salle;
    string nomNetbios;
    bool endScan;
    string getMACOutput(string nomNetbios);
    vector<pair<string, string>> getIPOutput();
public:
    scan();
    string getSalle();
    void scanMACIP(string salle);
    string sendRequete(string nomNetBios);
    void setSalle(string salle);
};

#endif // SCAN_H
