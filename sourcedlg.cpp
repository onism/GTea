#include "sourcedlg.h"
#include "ui_sourcedlg.h"
#include "global.h"
#include <QDebug>

SourceDlg::SourceDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SourceDlg)
{
    ui->setupUi(this);

    //设置参数
    ui->lineEdit->setText(Global::x_acoustic);
    ui->lineEdit_2->setText(Global::y_acoustic);
    ui->lineEdit_3->setText(Global::z_acoustic);
    ui->comboBox->setCurrentIndex(Global::p_type);
    switch(Global::p_type){
    case 0:
        ui->lineEdit_4->setText(Global::p1_acoustic);
        ui->lineEdit_7->setText(Global::p2_acoustic);
        break;
    case 1:
        break;
    case 2:
        ui->lineEdit_5->setText(Global::p1_acoustic);
        ui->lineEdit_6->setText(Global::p2_acoustic);
        break;
    default :
        break;

    }

}

SourceDlg::~SourceDlg()
{
    delete ui;
}



//提交参数
void SourceDlg::on_pushButton_clicked()
{
    Global::x_acoustic = ui->lineEdit->text();
    Global::y_acoustic = ui->lineEdit_2->text();
    Global::z_acoustic = ui->lineEdit_3->text();
    Global::p_type = ui->comboBox->currentIndex();
    switch(Global::p_type){
    case 0:
        Global::p1_acoustic = ui->lineEdit_4->text();
        Global::p2_acoustic = ui->lineEdit_7->text();
        break;
    case 1:
        break;
    case 2:
        Global::p1_acoustic = ui->lineEdit_5->text();
        Global::p2_acoustic = ui->lineEdit_6->text();
        break;
    default :
        break;

    }
    //关闭对话框
    accept();
}

void SourceDlg::on_pushButton_2_clicked()
{
    //关闭对话框
    reject();
}
