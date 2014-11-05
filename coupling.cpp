#include "coupling.h"
#include "ui_coupling.h"
#include <QSqlQuery>
#include "sqlhelper.h"
#include "global.h"
#include <QDebug>

Coupling::Coupling(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Coupling)
{
    ui->setupUi(this);

    QStringList listAco,listStr;
    QSqlQuery query(SQLHelper::dbLink);
    //acoustic的边界list
    query.exec("select filename,zonename,regname from cgns where type=='fluid' and btype<>6");
    while(query.next()){
        listAco<<query.value("filename").toString()+QString("--")\
                 +query.value("zonename").toString()+QString("--")\
                 +query.value("regname").toString();
    }
    //structure的边界list
    query.exec("select filename,zonename,regname from cgns where type= 'solid' and btype<>6");
    while(query.next()){
        listStr<<query.value("filename").toString()+QString("--")\
                 +query.value("zonename").toString()+QString("--")\
                 +query.value("regname").toString();
    }
    //把items插入list
    ui->listWidget->addItems(listAco);
    ui->listWidget_2->addItems(listStr);

    //listcoupling
    ui->listWidget_3->addItems(Global::listcoupling);

}

Coupling::~Coupling()
{
    delete ui;
}

//增加耦合边界
void Coupling::on_pushButton_clicked()
{
    QSqlQuery query(SQLHelper::dbLink);
    //判断两个list都不为空
    if(ui->listWidget->currentItem()!=NULL&&ui->listWidget_2->currentItem()!=NULL){
        QString str1 = ui->listWidget->currentItem()->text();
        QString str2 = ui->listWidget_2->currentItem()->text();

        if(str1.section("--",0,0)==str2.section("--",0,0)){
            //增加一对耦合边界
            ui->listWidget_3->addItem(str1+"+"+str2);

            //改变btype的值
            query.exec(QString("update cgns set btype=6 where filename = '%1' and zonename = '%2' and regname = '%3'")\
                       .arg(str1.section("--",0,0)).arg(str1.section("--",1,1)).arg(str1.section("--",2,2)));
            query.exec(QString("update cgns set btype=6 where filename = '%1' and zonename = '%2' and regname = '%3'")\
                       .arg(str2.section("--",0,0)).arg(str2.section("--",1,1)).arg(str2.section("--",2,2)));

            //删除原来的边界
            delete ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
            delete ui->listWidget_2->takeItem(ui->listWidget_2->row(ui->listWidget_2->currentItem()));
        }
    }
}

//删除耦合边界
void Coupling::on_pushButton_2_clicked()
{
    QSqlQuery query(SQLHelper::dbLink);
    if(ui->listWidget_3->currentItem()!=NULL){
        QString str1 = ui->listWidget_3->currentItem()->text().section("+",0,0);
        QString str2 = ui->listWidget_3->currentItem()->text().section("+",1,1);

        //还原
        ui->listWidget->addItem(str1);
        ui->listWidget_2->addItem(str2);

        //还原type
        query.exec(QString("update cgns set btype=4 where filename = '%1' and zonename = '%2' and regname = '%3'")\
                   .arg(str1.section("--",0,0)).arg(str1.section("--",1,1)).arg(str1.section("--",2,2)));
        query.exec(QString("update cgns set btype=4 where filename = '%1' and zonename = '%2' and regname = '%3'")\
                   .arg(str2.section("--",0,0)).arg(str2.section("--",1,1)).arg(str2.section("--",2,2)));


        //删除list3中对应item
        delete ui->listWidget_3->takeItem(ui->listWidget_3->row(ui->listWidget_3->currentItem()));

    }

}

void Coupling::on_pushButton_3_clicked()
{
    Global::listcoupling.clear();
    for(int i=0;i<ui->listWidget_3->count();++i){
        Global::listcoupling<<(ui->listWidget_3->item(i))->text();
    }
//    qDebug()<<Global::listcoupling;


    accept();
}


