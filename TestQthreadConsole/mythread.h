#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

class MyThread : public QThread
{
public:
    MyThread();
    void run();
    void setId(char);
private:
    char id;
};

#endif // MYTHREAD_H
