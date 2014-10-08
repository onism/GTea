#include "generalstr.h"
#include "ui_generalstr.h"

GeneralStr::GeneralStr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralStr)
{
    ui->setupUi(this);
}

GeneralStr::~GeneralStr()
{
    delete ui;
}
