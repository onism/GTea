#include "location.h"
#include "ui_location.h"
#include <QComboBox>

Location::Location(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Location)
{
    ui->setupUi(this);
    QComboBox *box = new QComboBox(this);
    QStringList list;
    list<<"ls1"<<"ls2"<<"ls3";
    box->addItems(list);
    ui->tableWidget->setCellWidget(0,0,box);
}

Location::~Location()
{
    delete ui;
}
