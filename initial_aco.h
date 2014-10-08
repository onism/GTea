#ifndef INITIAL_ACO_H
#define INITIAL_ACO_H

#include <QDialog>

namespace Ui {
class Initial_Aco;
}

class Initial_Aco : public QDialog
{
    Q_OBJECT

public:
    explicit Initial_Aco(QWidget *parent = 0);
    ~Initial_Aco();

private:
    Ui::Initial_Aco *ui;
};

#endif // INITIAL_ACO_H
