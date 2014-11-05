#include "displayform.h"
#include "ui_displayform.h"
#include "QFile"
#include "QTextStream"
#include "QIODevice"
#include "QDebug"

DisPlayForm::DisPlayForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisPlayForm)
{
    ui->setupUi(this);
    readInfo();
}

DisPlayForm::~DisPlayForm()
{
    delete ui;
}

void DisPlayForm::readInfo()
{
    QFile f("temTxt.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
            qDebug()<<"dkjfkdjfd";
        return ;
    }

    QTextStream txtInput(&f);
    QString lineStr;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        ui->textEdit->append(lineStr);
    }

    f.close();

}
