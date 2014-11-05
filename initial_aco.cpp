#include "initial_aco.h"
#include "ui_initial_aco.h"
#include "global.h"

Initial_Aco::Initial_Aco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Initial_Aco)
{
    ui->setupUi(this);

    ui->lineEdit->setText(Global::pressure);
    ui->lineEdit_2->setText(Global::u_acoustic);
    ui->lineEdit_3->setText(Global::v_acoustic);
    ui->lineEdit_4->setText(Global::w_acoustic);

}

Initial_Aco::~Initial_Aco()
{
    delete ui;
}

void Initial_Aco::on_pushButton_clicked()
{
    Global::pressure = ui->lineEdit->text();
    Global::u_acoustic = ui->lineEdit_2->text();
    Global::v_acoustic = ui->lineEdit_3->text();
    Global::w_acoustic = ui->lineEdit_4->text();

    accept();
}

void Initial_Aco::on_pushButton_2_clicked()
{
    reject();
}
