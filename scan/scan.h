#ifndef SCAN_H
#define SCAN_H
#include <string>
#include <iostream>
#include <regex>
#include <QThread>
#include <atomic>

using namespace std;

class scan : public QThread
{
private:
    int PC = 1;
    int PCMax = 50;
    string salle = "A402";
    string** list_temp;
    string getMACOutput(string nomNetbios);
    vector<pair<string, string>> getIPOutput();
    void executeScan();

public:
    scan(int nPC);
    string getSalle();
    bool scanMACIP(string salle);
    void run();
};

#endif // SCAN_H
