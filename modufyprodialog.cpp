#include "modufyprodialog.h"
#include "ui_modufyprodialog.h"
#include "sqlhelper.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
ModufyProDialog::ModufyProDialog(QWidget *parent) :
    QDialog(parent),
    fromfilename(""),
    ui(new Ui::ModufyProDialog)
{
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList() << "name" << "property");
}

ModufyProDialog::~ModufyProDialog()
{
    delete ui;
}


void ModufyProDialog::setItem(QString filename)
{
    QString sql = "";
    fromfilename = filename;
    if(filename.endsWith("cgns"))
    {
        sql = "select distinct zonename,type from cgns where filename = '"+filename+"' and output!=1";
    }else if(filename.endsWith("msh"))
    {
        sql = "select distinct regname,type from cgns where filename = '"+filename+"'and output<>1";
    }
    qDebug()<<sql;
    QList<QString> zonenamelist;
    QList<QString> typelist;
    if (SQLHelper::dbLink.isOpen())
    {
        QSqlQuery query;
        query.exec(sql);
        while (query.next()) {
            QString name = query.value(0).toString();
            zonenamelist.push_back(name);
            QString type = query.value(1).toString();
            typelist.push_back(type);
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0,  name);
            item->setText(1, type);
            ui->treeWidget->addTopLevelItem(item);
        }
    }
}

void ModufyProDialog::on_buttonBox_clicked(QAbstractButton *button)
{
      if (QString::compare(button->text(),"OK", Qt::CaseInsensitive) == 0){

          emit emit_signal_update();
      }else
      {

      }
}
