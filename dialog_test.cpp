#include "dialog_test.h"
#include "ui_dialog_test.h"
#include "addboundaco.h"

Dialog_test::Dialog_test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_test)
{
    ui->setupUi(this);




}

Dialog_test::~Dialog_test()
{
    delete ui;
}





void Dialog_test::on_pushButton_3_clicked()
{
    AddBoundAco aba;
    ui->listWidget_2->addItem(ui->listWidget->currentItem());

}
