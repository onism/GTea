#include "initial_str.h"
#include "ui_initial_str.h"
#include "global.h"

Initial_Str::Initial_Str(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Initial_Str)
{
    ui->setupUi(this);

    ui->lineEdit->setText(Global::x_move_structure);
    ui->lineEdit_2->setText(Global::y_move_structure);
    ui->lineEdit_3->setText(Global::z_move_structure);
}

Initial_Str::~Initial_Str()
{
    delete ui;
}

void Initial_Str::on_pushButton_clicked()
{
    Global::x_move_structure = ui->lineEdit->text();
    Global::y_move_structure = ui->lineEdit_2->text();
    Global::z_move_structure = ui->lineEdit_3->text();

    accept();
}

void Initial_Str::on_pushButton_2_clicked()
{
    reject();
}
