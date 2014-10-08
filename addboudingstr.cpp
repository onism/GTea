#include "addboudingstr.h"
#include "ui_addboudingstr.h"

AddBoudingStr::AddBoudingStr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBoudingStr)
{
    ui->setupUi(this);
}

AddBoudingStr::~AddBoudingStr()
{
    delete ui;
}
