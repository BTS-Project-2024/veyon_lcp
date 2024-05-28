#include "ihm.h"
#include "ui_ihm.h"

ihm::ihm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ihm)
{
    ui->setupUi(this);
}

ihm::~ihm()
{
    delete ui;
}
