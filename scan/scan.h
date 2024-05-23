#ifndef SCAN_H
#define SCAN_H
#include <string>
#include <iostream>
#include <regex>
#include <QThread>
#include <threadpool.h>

using namespace std;

class scan : public thread
{
private:
    ThreadPool ThreadScan;
    string salle = "A402";
    int incTab=0;
    string** list_temp;
    string getMACOutput(string nomNetbios);
    vector<pair<string, string>> getIPOutput();
    bool scanMACIP(string nomNetbios);
    void run_scan(int PC);
    void clean_tab();
    mutex mtx;
public:
    scan();
    void setSalle();
    string getSalle();
    string** getTab();
    void run_tscan();
};

#endif // SCAN_H
