#ifndef GENERALACO_H
#define GENERALACO_H

#include <QDialog>

namespace Ui {
class GeneralAco;
}

class GeneralAco : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralAco(QWidget *parent = 0);
    ~GeneralAco();

private:
    Ui::GeneralAco *ui;
};

#endif // GENERALACO_H
