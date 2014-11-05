#ifndef LOCATION_H
#define LOCATION_H

#include <QDialog>
#include "QSqlDatabase"
#include "QSqlQuery"
#include "QDir"
#include <QComboBox>
#include <QDebug>

namespace Ui {
class Location;
}

class Location : public QDialog
{
    Q_OBJECT

public:
    explicit Location(int rowCount=0,QWidget *parent = 0);
    ~Location();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Location *ui;
    int count;  //设置点数
    void initComboBox(QComboBox *box);
    void WriteToSQL();
};

#endif // LOCATION_H
