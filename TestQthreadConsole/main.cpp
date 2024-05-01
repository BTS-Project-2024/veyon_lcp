#include <QCoreApplication>
#include <iostream>
#include <QThread>
#include "mythread.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout << "Test de QThread" << endl;

    MyThread t1;
    t1.setId('A');
    t1.start();

    //t1.wait();
    MyThread t2;
    t2.setId('B');
    t2.start();

    //t2.wait();
    MyThread t3;
    t3.setId('C');
    t3.start();



//    // creation de 10 threads avec boucle et tableau
//    MyThread * tabThread[10];
//    for(int i=0; i<10; i++)
//    {
//        tabThread[i] = new MyThread();
//        tabThread[i]->setId('E'+i);
//    }

//    // demarrage des 10 threads avec boucle et tableau
//    for(int i=0; i<10; i++)
//    {
//        tabThread[i]->start();
//    }



    return a.exec();
}

