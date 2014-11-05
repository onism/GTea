#include "location.h"
#include "ui_location.h"
#include "sqlhelper.h"


Location::Location(int rowCount,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Location),count(rowCount)
{
    ui->setupUi(this);


    //初始化表格
    for(int i=0;i<count;++i)
    {
      ui->tableWidget->insertRow(0);
      QComboBox *box1 = new QComboBox(this);
    //  QStringList list;
     // list<<"fluid"<<"solid";
     // box1->addItems(list);
      initComboBox(box1);
      ui->tableWidget->setCellWidget(0,0,box1);
      ui->tableWidget->setItem(0,1,new QTableWidgetItem("0"));
      ui->tableWidget->setItem(0,2,new QTableWidgetItem("0"));
      ui->tableWidget->setItem(0,3,new QTableWidgetItem("0"));
      ui->tableWidget->resizeColumnToContents(0); //根据内容自适应
    }

}

Location::~Location()
{
    delete ui;
}

void Location::initComboBox(QComboBox *box)
{


    QSqlQuery queryName(SQLHelper::dbLink);
    queryName.exec(QString("select Name from FieldNameLocation"));  //读取数据库目标集

    while(queryName.next())
    {
        box->addItem(queryName.value(0).toString());
    }

   // db.close();
}

//写入数据库按钮
void Location::on_pushButton_clicked()
{
WriteToSQL();
}

void Location::WriteToSQL()
{

    QSqlQuery queryFieldName(SQLHelper::dbLink);
    queryFieldName.clear();
    queryFieldName.prepare("update FieldNameLocation set X=:xVal,Y=:yVal,Z=:zVal where Name=:NameVal");

    for(int i=0;i<count;++i)
    {
    //获得域名字的值
     QWidget *widget=ui->tableWidget->cellWidget(i,0);
     QComboBox *comboBox=(QComboBox *)widget;
     QString NameStr=comboBox->currentText();
     qDebug()<<NameStr;

     QString XStr=ui->tableWidget->item(i,1)->text();
     QString YStr=ui->tableWidget->item(i,2)->text();
     QString ZStr=ui->tableWidget->item(i,3)->text();

     queryFieldName.bindValue(":xVal",XStr.toDouble());
     queryFieldName.bindValue(":yVal",YStr.toDouble());
     queryFieldName.bindValue(":zVal",ZStr.toDouble());
     queryFieldName.bindValue(":NameVal",NameStr);

     queryFieldName.exec();
    }

    this->close();
}

void Location::on_pushButton_2_clicked()
{
    this->close();
}
