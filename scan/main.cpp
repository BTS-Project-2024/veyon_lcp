#include "scan.h"
#include "ihm.h"
#include "ui_ihm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ihm w;
    w.show();
    return a.exec();
}
