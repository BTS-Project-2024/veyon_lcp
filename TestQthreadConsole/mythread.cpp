#include "mythread.h"
#include <iostream>
#include <time.h>
#include <QMutex>
#include <QSemaphore>


using namespace std;

MyThread::MyThread()
{}

void MyThread::run()
{
    for(int i=1; i<= 1000; i++)
    {
        cout << id << i << " ";
        //Sleep(500);
    }
}

void MyThread::setId(char unId)
{
    id = unId;
}
