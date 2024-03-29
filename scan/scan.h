#ifndef SCAN_H
#define SCAN_H
#include <string>
#include <iostream>
#include <regex>

using namespace std;

class scan
{
private:
    string** list_temp;
    bool endScan;
    string getMACOutput(string nomNetbios);
    vector<pair<string, string>> getIPOutput();
public:
    scan(int maxPC, string salle);
    string getSalle();
    bool scanMACIP(string salle);
    string sendRequete(string nomNetBios);
    void setSalle(string salle);
};

#endif // SCAN_H
