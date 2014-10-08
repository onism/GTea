#include "initial_str.h"
#include "ui_initial_str.h"

Initial_Str::Initial_Str(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Initial_Str)
{
    ui->setupUi(this);
}

Initial_Str::~Initial_Str()
{
    delete ui;
}
