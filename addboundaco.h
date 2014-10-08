#ifndef ADDBOUNDACO_H
#define ADDBOUNDACO_H

#include <QDialog>
#include <QList>
#include <QAbstractButton>
namespace Ui {
class AddBoundAco;
}

class AddBoundAco : public QDialog
{
    Q_OBJECT

public:
    explicit AddBoundAco(QWidget *parent = 0);
    ~AddBoundAco();
    QString str;
    void setItem(QList<QString> structure);
private:
    Ui::AddBoundAco *ui;
private slots:
    void getcurrenttex();

    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // ADDBOUNDACO_H
