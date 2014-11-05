#include "addboundaco.h"
#include "ui_addboundaco.h"
#include "global.h"
#include "sqlhelper.h"
#include <QSqlQuery>
#include <QMessageBox>

AddBoundAco::AddBoundAco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBoundAco)
{
    ui->setupUi(this);

    QSqlQuery query(SQLHelper::dbLink);
    QString str = Global::bounding->text(0);
    int btype,ptype;
    QString  p1,p2;

    query.exec(QString("select btype,ptype,p1,p2 from cgns where filename='%1' and zonename='%2' and regname='%3'")\
               .arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
    while(query.next()){
        btype = query.value("btype").toInt();
        ptype = query.value("ptype").toInt();
        p1 = query.value("p1").toString();
        p2=query.value("p2").toString();
    }

    switch(btype){
    //压力边界
    case 1:
        ui->comboBox->setCurrentIndex(0);
        switch(ptype){
        //高斯
        case 0:
            ui->comboBox_3->setCurrentIndex(0);
            ui->lineEdit_4->setText(p1);
            ui->lineEdit_7->setText(p2);
            break;
        //白噪声
        case 1:
            ui->comboBox_3->setCurrentIndex(1);
            break;
        //单频
        case 2:
            ui->comboBox_3->setCurrentIndex(2);
            ui->lineEdit_5->setText(p1);
            ui->lineEdit_6->setText(p2);
            break;
        default:
            break;
        }
        break;
    //绝对软边界
    case 2:
        ui->comboBox->setCurrentIndex(1);
        break;
    //绝对硬边界
    case 4:
        ui->comboBox->setCurrentIndex(2);
        break;
    //透射边界
    case 3:
        ui->comboBox->setCurrentIndex(3);
        break;

    default :
        break;
    }



}


AddBoundAco::~AddBoundAco()
{
    delete ui;
}



void AddBoundAco::on_pushButton_clicked()
{
    QSqlQuery query(SQLHelper::dbLink);
    QString str = Global::bounding->text(0);
    int btype,ptype = 0;
    QString  p1,p2;

    switch(ui->comboBox->currentIndex()){
    //压力
    case 0:
        btype = 1;
        switch(ui->comboBox_3->currentIndex()){
        //高斯
        case 0:
            ptype = 0;
            p1 = ui->lineEdit_4->text();
            p2 = ui->lineEdit_7->text();
            if(p1==""||p2=="")
                QMessageBox::warning(this,"Warning!","Please input the value!");
            else{
                query.exec(QString("update cgns set flag = 1,btype = %1,ptype = %2,p1 = %3,p2 = %4 where filename = '%5' and zonename = '%6' and regname = '%7'")\
                           .arg(btype).arg(ptype).arg(p1).arg(p2).arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
                accept();
            }
            break;
        //白噪声
        case 1:
            ptype = 1;
            query.exec(QString("update cgns set flag = 1,btype = %1,ptype = %2 where filename = '%5' and zonename = '%6' and regname = '%7'")\
                       .arg(btype).arg(ptype).arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
            accept();
            break;            
        //单频
        case 2:
            ptype = 2;
            p1 = ui->lineEdit_5->text();
            p2 = ui->lineEdit_6->text();
            if(p1==""||p2=="")
                QMessageBox::warning(this,"Warning!","Please input the value!");
            else{
                query.exec(QString("update cgns set flag = 1,btype = %1,ptype = %2,p1 = %3,p2 = %4 where filename = '%5' and zonename = '%6' and regname = '%7'")\
                           .arg(btype).arg(ptype).arg(p1).arg(p2).arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
                accept();
            }
            break;
        default :
            break;
        }
        break;
    //绝对软边界
    case 1:
        btype = 2;
        query.exec(QString("update cgns set flag = 1,btype = %1 where filename = '%5' and zonename = '%6' and regname = '%7'")\
                   .arg(btype).arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
        accept();
        break;
    //绝对硬边界
    case 2:
        btype = 4;
        query.exec(QString("update cgns set flag = 1,btype = %1 where filename = '%5' and zonename = '%6' and regname = '%7'")\
                   .arg(btype).arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
        accept();
        break;
    //透射
    case 3:
        btype = 3;
        query.exec(QString("update cgns set flag = 1,btype = %1 where filename = '%5' and zonename = '%6' and regname = '%7'")\
                   .arg(btype).arg(str.section("--",0,0)).arg(str.section("--",1,1)).arg(str.section("--",2,2)));
        accept();
        break;
    default :
        break;


    }




}

void AddBoundAco::on_pushButton_2_clicked()
{
    reject();
}
