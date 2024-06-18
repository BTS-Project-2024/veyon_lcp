#ifndef SCAN_H
#define SCAN_H
#include <string>
#include <iostream>
#include <regex>
#include <thread>
#include <threadpool.h>

using namespace std;

class Scan
{
private:
    ThreadPool ThreadScan;
    string salle;
    int incTab=0;
    string** list_pc;
    string list_temp[50][3];
    string getMACOutput(string);
    vector<pair<string, string>> getIPOutput();
    bool scanMACIP(string);
    void run_scan(int);
    void clean_tab();
    mutex mtx;
public:
    Scan();
    void setSalle(string);
    string getSalle();
    string** getTab();
    int getTabSize();
    void run_tscan();
};

#endif // SCAN_H
