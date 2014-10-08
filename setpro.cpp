#include "setpro.h"
#include "ui_setpro.h"
#include <QMessageBox>
SetPro::SetPro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetPro)
{
    ui->setupUi(this);
}

SetPro::~SetPro()
{
    delete ui;
}


void SetPro::setItem(QList<QString> zone_names)
{
    ui->comboBox->clear();
    int n = zone_names.size();
    if( n > 0 ){
       for(int i = 0; i < n; i++){
           ui->comboBox->addItem(zone_names.at(i));
       }
    }else{
       QMessageBox::information(NULL, tr("sorry"), tr("You didn't have any zones."));
    }
}
