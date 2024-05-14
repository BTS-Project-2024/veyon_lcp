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
    int incTab=0;
    string list_temp[50][3];
    string getMACOutput(string nomNetbios);
    vector<pair<string, string>> getIPOutput();
    bool scanMACIP(string salle);
    void run_scan(int PC);
    void clean_tab();
    mutex mtx;
public:
    string getSalle();
    void getTab();
    void run_tscan();
};

#endif // SCAN_H
