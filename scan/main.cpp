#include <QCoreApplication>
#include <iostream>
#include <thread>
#include <windows.h>
#include "scan.h"
#include "csv.h"
#include "config.h"

using namespace std;


int main() {
    scan testscan;
    testscan.run_tscan();
    testscan.getTab();
    return 0;
}
