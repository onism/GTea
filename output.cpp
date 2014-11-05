#include "output.h"
#include "ui_output.h"
#include "location.h"
#include "displayform.h"
#include "QDebug"
#include "QFileDialog"
#include "QMessageBox"
#include "QTextStream"
#include "QFile"
#include "QIODevice"

Output::Output(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Output)
{
    ui->setupUi(this);
    InitComboBox();
}

Output::~Output()
{
    delete ui;
}

void Output::on_pushButton_clicked()
{
    int count = ui->lineEdit_2->text().toInt();

     Location *loc = new Location(count);
                 //loc->setCount(count);
     loc->exec();

}


//初始化ComboBox
void Output::InitComboBox()
{

    QSqlQuery queryName(SQLHelper::dbLink);
    queryName.exec(QString("select Name from FieldNameLocation"));  //读取数据库目标集

    while(queryName.next())
    {
        ui->comboBox->addItem(queryName.value(0).toString());
    }

   // db.close();
}

void Output::on_comboBox_2_currentIndexChanged(const QString &arg1)
{



}

void Output::on_pushButton_2_clicked()
{
    WriteTxt("temTxt.txt"); //生成一个临时文件
    DisPlayForm *disPlay=new DisPlayForm(this);

    switch (ui->comboBox_2->currentIndex()) {
    case 1:
         SaveFile();
        break;
    case 2:
        disPlay->show();
        break;
    case 3:
        SaveFile();
        disPlay->show();
        break;
    default:
        QMessageBox::warning(this,tr("Path error"),tr("没有选择正确的文件！"));
        break;
    }
  //  QFile::remove("temTxt.txt");//删除临时文件
}

void Output::SaveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("保存文件"),
            "",
            tr("Text files (*.txt)"));

        if (!fileName.isNull())
        {
          //qDebug()<<fileName;
          // WriteTxt(fileName);
            copyFile("temTxt.txt",fileName,true);
        }
        else
        {
            QMessageBox::warning(this,tr("Path error"),tr("没有选择正确的文件！"));
        }
}

void Output::WriteTxt(QString filePath)
{

    QSqlQuery queryName(SQLHelper::dbLink);


    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
            QMessageBox::critical(NULL,"提示","无法创建文件!");
    }
    else
    {
     QTextStream out(&file);
     out<<"Ring case"<<endl;
     out<<"DUMMY2"<<endl;
     out<<"**The following section for phase info for each zone**"<<endl;
     out<<"NREC=1 NUM OF CGNS FILES"<<endl;
     out<<"  1"<<endl;
     out<<"NREC=1 CGNS FILE NAME"<<endl;

     //写入文件名
     queryName.exec(QString("select DISTINCT filename from cgns"));  //读取数据库目标集
     while(queryName.next())
     {
        // box->addItem(queryName.value(0).toString());
       out<<"    ";  out<<queryName.value(0).toString();
     }
     out<<"   2"<<endl;


     out<<"NREC=1 NUMBER OF ZONES"<<endl;

     //子域
     queryName.exec(QString("select count(*)  from cgns where output=1"));
     while(queryName.next())
     {
        // box->addItem(queryName.value(0).toString());
       out<<"    ";  out<<queryName.value(0).toString();
     }
       out<<"  1"<<endl;
       out<<"    6  3"<<endl;

     out<<"NREC=1 AXI2D 3D 2D REVOLVE"<<endl;
     out<<"      F T F F"<<endl;
     out<<"NREC=1 GRID SCALE"<<endl;
     out<<"     1"<<endl;
     out<<"NREC=1 INTERIOR FACES between zones in ONE CGNS file"<<endl;
     out<<"          F"<<endl;    //####
     out<<"NREC=1 NINFACE PAIR AND NCYC_PAIR"<<endl;
     out<<"      1  0"<<endl;     //###
     out<<"**end head info**"<<endl;
     out<<"**Problem type**"<<endl;
     out<<"NREC=1 TRANSIENT"<<endl;

    out<<"     4     0       0"<<endl;
    out<<"     0.0   2.0e-4  1.0e-8"<<endl;//###
    out<<"       1.0e-8   1.0e-8"<<endl;//###

    out<<"NREC=1 WAVE EQUATION Flag for solving wave equation + number of variables"<<endl;
    out<<"     5   4"<<endl;
    out<<"NREC=1 INITIAL FIELD  for each phase"<<endl;
    out<<"     2.775    0.0    0.0    0.0     0.0      300.0    0.01        1"<<endl;
    out<<"**Boundary Conditions**  name/type/value(u v w p)"<<endl;
    out<<"NREC=1 THINWALL"<<endl;
    out<<"     F    F"<<endl;
    out<<"NREC=1 BOUNDARY"<<endl;

    queryName.exec(QString("select regname  from cgns"));
    while(queryName.next())
    {
       // box->addItem(queryName.value(0).toString());
      out<<"   ";  out<<queryName.value(0).toString();
    }
    out<<""<<endl;//###

    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"0.0   0.0   0.0   0.0   0.0   0.0   0   1.2   0.01    1.0   300   0.0"<<endl;
    out<<"**output control max_prts prt_set**"<<endl;
    out<<"NREC=1 OUTPUT"<<endl;

    out<<"     F   F    T "<<endl;//###
    out<<"     F"<<endl;//###
    out<<"     20    0"<<endl;
    out<<"     ";out<<ui->lineEdit->text()<<endl;//输出次数

    out<<"NREC=";out<<ui->lineEdit_2->text();out<<" POINTERS MON"<<endl;
    out<<"       0"<<endl;

    queryName.exec(QString("select X,Y,Z  from FieldNameLocation"));
    while(queryName.next())
    {
      out<<"   ";  out<<queryName.value(0).toString();
      out<<"   ";  out<<queryName.value(1).toString();
      out<<"   ";  out<<queryName.value(2).toString();
      out<<"   ";  out<<"";//###
      out<<endl;
    }
     out<<"END"<<endl;

     out.flush();
     file.close();
    }
}


bool Output::copyFile(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
   // toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }
    QDir *createfile     = new QDir;
    bool exist = createfile->exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile->remove(toDir);
        }
    }//end if

    if(!QFile::copy(sourceDir, toDir))
    {
        return false;
    }
    return true;
}
