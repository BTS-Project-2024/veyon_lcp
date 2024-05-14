#ifndef SCAN_H
#define SCAN_H
#include <string>
#include <iostream>
#include <regex>
#include <QThread>
#include <atomic>
#include <threadpool.h>

using namespace std;

class scan : public thread
{
private:
    ThreadPool ThreadScan;
    string salle = "A402";
    string** list_temp;
    string getMACOutput(string nomNetbios);
    vector<pair<string, string>> getIPOutput();
    bool scanMACIP(string salle);
    void run_scan(int PC);
public:
    string getSalle();
    void run_tscan();
};

#endif // SCAN_H
