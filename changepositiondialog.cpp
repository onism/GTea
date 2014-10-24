#include "changepositiondialog.h"
#include "ui_changepositiondialog.h"
#include "sqlhelper.h"
#include <QDebug>
#include <QRegExp>
#include <QRegExpValidator>
ChangePositionDialog::ChangePositionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePositionDialog)
{
    ui->setupUi(this);

   // QRegExp regExp2("[0-9]{1,2}[.][0-9]{0,2}");
    QRegExp regExpfloat("^(-?\\d+)(\\.\\d+)?$");
    QRegExpValidator *pRegExpValidatorfloat = new QRegExpValidator(regExpfloat,this);
    ui->angle_x->setValidator(pRegExpValidatorfloat);
    ui->angle_y->setValidator(pRegExpValidatorfloat);
    ui->angle_z->setValidator(pRegExpValidatorfloat);
    ui->scale_x->setValidator(pRegExpValidatorfloat);
    ui->scale_y->setValidator(pRegExpValidatorfloat);
    ui->scale_z->setValidator(pRegExpValidatorfloat);
    ui->trans_x->setValidator(pRegExpValidatorfloat);
    ui->trans_y->setValidator(pRegExpValidatorfloat);
    ui->trans_z->setValidator(pRegExpValidatorfloat);

}

void ChangePositionDialog::setItem()
{
    ui->angle_x->setText("0.0");
    ui->angle_y->setText("0.0");
    ui->angle_z->setText("0.0");
    ui->trans_x->setText("0.0");
    ui->trans_y->setText("0.0");
    ui->trans_z->setText("0.0");
    ui->scale_x->setText("1.0");
    ui->scale_y->setText("1.0");
    ui->scale_z->setText("1.0");
    QString select_filename_cgns = "select distinct filename from cgns ";
    QString select_filename_msh = "select distinct filename from msh ";
    QList<QString> filelist_cgns = SQLHelper::selectbysql(select_filename_cgns);
    QList<QString> filelist_msh  = SQLHelper::selectbysql(select_filename_msh);
    ui->comboBox->clear();
    filelist_cgns.isEmpty();
    int n =0;

    if(! filelist_cgns.isEmpty() ){
        n = filelist_cgns.size();
      for(int i = 0; i < n; i++){
              ui->comboBox->addItem(filelist_cgns.at(i));
      }
     }

    if(! filelist_msh.isEmpty() ){
        n = filelist_msh.size();
        for(int i = 0; i < n; i++){
         ui->comboBox->addItem(filelist_msh.at(i));
       }
    }
}

ChangePositionDialog::~ChangePositionDialog()
{
    delete ui;
}



ChangeObject::ChangeObject(QString filename,QString zonename):
    x_scale(1.0),
    y_scale(1.0),
    z_scale(1.0),
    trans_x(0.0),
    trans_y(0.0),
    trans_z(0.0),
    angle_x(0.0),
    angle_y(0.0),
    angle_z(0.0)
{
    this->filename = filename;
    this->zonename = zonename;
}

void ChangePositionDialog::on_buttonBox_clicked(QAbstractButton *button)
{
   if (QString::compare(button->text(),"OK", Qt::CaseInsensitive) == 0){
    QString filename = ui->comboBox->currentText();
    QString zonename = ui->comboBox_2->currentText();

    ChangeObject object(filename,zonename);
    object.angle_x = ui->angle_x->text().toFloat();
    object.angle_y = ui->angle_y->text().toFloat();
    object.angle_z = ui->angle_z->text().toFloat();
    object.trans_x = ui->trans_x->text().toFloat();
    object.trans_y = ui->trans_y->text().toFloat();
    object.trans_z = ui->trans_z->text().toFloat();
    object.x_scale = ui->scale_x->text().toFloat();
    object.y_scale = ui->scale_y->text().toFloat();
    object.z_scale = ui->scale_z->text().toFloat();
//    object.x_scale = ui->lineEdit->text().toFloat();
//    object.y_scale = ui->lineEdit_2->text().toFloat();
//    object.z_scale = ui->lineEdit_3->text().toFloat();
//    object.angle_scale = ui->lineEdit_4->text().toFloat();
//    float x = ui->lineEdit->text().toFloat();
//    float y = ui->lineEdit_2->text().toFloat();
//    float z = ui->lineEdit_3->text().toFloat();
//    float angle = ui->lineEdit_4->text().toFloat();
     emit update_position(object );
   //emit update_position(filename,zonename,x,y,z,angle);
   }

}

void ChangePositionDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->comboBox_2->clear();
    QString selectzonesql = "select distinct zonename from ";
    QList<QString> zonelists;
    if(arg1.endsWith("cgns")){
        selectzonesql += "cgns ";
        selectzonesql +="where filename = '";
        selectzonesql += arg1;
        selectzonesql += "'";
        zonelists = SQLHelper::selectbysql(selectzonesql);
    }else{
        selectzonesql += "msh ";
        selectzonesql +="where filename = '";
        selectzonesql += arg1;
        selectzonesql += "'";
        zonelists = SQLHelper::selectbysql(selectzonesql);
    }
    qDebug()<<selectzonesql;
    if(!zonelists.isEmpty()){
    int n = zonelists.size();
    if( n > 0 ){
      for(int i = 0; i < n; i++){
              ui->comboBox_2->addItem(zonelists.at(i));
      }
     }
    }
}
