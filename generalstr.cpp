#include "generalstr.h"
#include "ui_generalstr.h"
#include "global.h"

GeneralStr::GeneralStr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralStr)
{
    ui->setupUi(this);

    ui->lineEdit->setText(Global::time_start_structure);
    ui->lineEdit_2->setText(Global::time_stop_structure);
    ui->lineEdit_3->setText(Global::time_step_structure);
}

GeneralStr::~GeneralStr()
{
    delete ui;
}

void GeneralStr::on_pushButton_clicked()
{
    Global::time_start_structure = ui->lineEdit->text();
    Global::time_stop_structure = ui->lineEdit_2->text();
    Global::time_step_structure = ui->lineEdit_3->text();

    accept();
}

void GeneralStr::on_pushButton_2_clicked()
{
    reject();
}
