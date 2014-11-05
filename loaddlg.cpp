#include "loaddlg.h"
#include "ui_loaddlg.h"
#include "global.h"

LoadDlg::LoadDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadDlg)
{
    ui->setupUi(this);

    ui->lineEdit->setText(Global::x_structure);
    ui->lineEdit_2->setText(Global::y_structure);
    ui->lineEdit_3->setText(Global::z_structure);
    ui->comboBox->setCurrentIndex(Global::load_type);
    switch(Global::load_type){
    case 0:
        ui->lineEdit_4->setText(Global::p1_structure);
        ui->lineEdit_7->setText(Global::p2_structure);
        break;
    case 1:
        break;
    case 2:
        ui->lineEdit_5->setText(Global::p1_structure);
        ui->lineEdit_6->setText(Global::p2_structure);
        break;
    default :
        break;

    }

}

LoadDlg::~LoadDlg()
{
    delete ui;
}

void LoadDlg::on_pushButton_clicked()
{

    Global::x_structure = ui->lineEdit->text();
    Global::y_structure = ui->lineEdit_2->text();
    Global::z_structure = ui->lineEdit_3->text();
    Global::load_type = ui->comboBox->currentIndex();
    switch(Global::load_type){
    case 0:
        Global::p1_structure = ui->lineEdit_4->text();
        Global::p2_structure = ui->lineEdit_7->text();
        break;
    case 1:
        break;
    case 2:
        Global::p1_structure = ui->lineEdit_5->text();
        Global::p2_structure = ui->lineEdit_6->text();
        break;
    default :
        break;

    }

    accept();
}

void LoadDlg::on_pushButton_2_clicked()
{
    reject();
}
