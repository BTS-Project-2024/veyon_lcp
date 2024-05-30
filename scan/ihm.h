#ifndef IHM_H
#define IHM_H

#include <QDialog>
#include "scan.h"
#include "csv.h"
#include "config.h"

namespace Ui {
class ihm;
}

class ihm : public QDialog
{
    Q_OBJECT

public:
    explicit ihm(QWidget *parent = nullptr);
    ~ihm();

private slots:
    void on_scanButton_clicked();

private:
    Ui::ihm *ui;
    scan *scanObj;
    CSV *csvObj;
    Config *configObj;
};

#endif // IHM_H
