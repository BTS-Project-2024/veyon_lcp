#include <QCoreApplication>
#include <iostream>
#include <thread>
#include <windows.h>
#include "scan.h"

using namespace std;


int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);

    scan * tabThread[100];

    for(int i=0; i<99; i++)
    {
        tabThread[i] = new scan(i+1);
    }

    for(int i=0; i<=99; i++)
    {
        tabThread[i]->start();
        Sleep(200);
    }

    return a.exec();
}
