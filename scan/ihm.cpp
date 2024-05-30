#include "ihm.h"
#include "ui_ihm.h"
#include "scan.h"

ihm::ihm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ihm)
{
    ui->setupUi(this);
    scanObj = new scan(ui->);
    csvObj = new CSV();
    configObj = new Config();

}

ihm::~ihm()
{
    delete ui;
}

void ihm::on_scanButton_clicked()
{
    // recup nom salle
    string s;
    s = ui->salleText->text().toStdString();

    scanObj->setSalle(s);
    scanObj->run_tscan();
}
