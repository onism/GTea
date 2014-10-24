#include "output.h"
#include "ui_output.h"
#include "location.h"

Output::Output(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
}

Output::~Output()
{
    delete ui;
}

void Output::on_pushButton_clicked()
{
//    int count = ui->lineEdit_2->text().toInt();

        Location *loc = new Location();
        loc->exec();

}
