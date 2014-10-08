#include "dialog.h"
#include "ui_dialog.h"
#include <QDateTime>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("hh:mm:ss");
    ui->label_4->setText(str);


}

Dialog::~Dialog()
{
    delete ui;
}




