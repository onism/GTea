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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::GeneralAco *ui;
};

#endif // GENERALACO_H
