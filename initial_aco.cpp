#include "initial_aco.h"
#include "ui_initial_aco.h"

Initial_Aco::Initial_Aco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Initial_Aco)
{
    ui->setupUi(this);
}

Initial_Aco::~Initial_Aco()
{
    delete ui;
}
