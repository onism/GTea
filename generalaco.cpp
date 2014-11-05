#include "generalaco.h"
#include "ui_generalaco.h"
#include "global.h"

GeneralAco::GeneralAco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralAco)
{
    ui->setupUi(this);

    ui->lineEdit->setText(Global::time_start_acoustic);
    ui->lineEdit_2->setText(Global::time_stop_acoustic);
    ui->lineEdit_3->setText(Global::time_step_acoustic);
}

GeneralAco::~GeneralAco()
{
    delete ui;
}

void GeneralAco::on_pushButton_clicked()
{
    Global::time_start_acoustic = ui->lineEdit->text();
    Global::time_stop_acoustic = ui->lineEdit_2->text();
    Global::time_step_acoustic = ui->lineEdit_3->text();

    accept();

}

void GeneralAco::on_pushButton_2_clicked()
{
    reject();
}
