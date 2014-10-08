#include "generalaco.h"
#include "ui_generalaco.h"

GeneralAco::GeneralAco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralAco)
{
    ui->setupUi(this);
}

GeneralAco::~GeneralAco()
{
    delete ui;
}
