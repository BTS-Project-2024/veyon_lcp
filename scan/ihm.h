#ifndef IHM_H
#define IHM_H

#include <QDialog>

namespace Ui {
class ihm;
}

class ihm : public QDialog
{
    Q_OBJECT

public:
    explicit ihm(QWidget *parent = nullptr);
    ~ihm();

private:
    Ui::ihm *ui;
};

#endif // IHM_H
