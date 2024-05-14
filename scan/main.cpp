#include <QCoreApplication>
#include <iostream>
#include <thread>
#include <windows.h>
#include "scan.h"

using namespace std;


int main() {
    scan testscan;
    testscan.run_tscan();
    return 0;
}
