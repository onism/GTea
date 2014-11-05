#ifndef COUPLING_H
#define COUPLING_H

#include <QDialog>

namespace Ui {
class Coupling;
}

class Coupling : public QDialog
{
    Q_OBJECT

public:
    explicit Coupling(QWidget *parent = 0);
    ~Coupling();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Coupling *ui;
};

#endif // COUPLING_H
