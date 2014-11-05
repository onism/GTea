#include "modufyprodialog.h"
#include "ui_modufyprodialog.h"
#include "sqlhelper.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
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
void ModufyProDialog::clearItem()
{
    ui->treeWidget->clear();
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
          QTreeWidgetItemIterator it(ui->treeWidget);
              while (*it) {
                   //do something like
                  QString zonename = (*it)->text(0);
                  QString property = (*it)->text(1);
                   qDebug()<<property;
                  if(property!="fluid"&&property!="solid")
                  {
                      QMessageBox::critical(NULL, "critical", "must set fluid or solid", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                      return;
                  }
                  QString sql = "";
                  if(fromfilename.endsWith("cgns")){
                      sql += "update cgns set type = '";
                      sql += property;
                      sql +="' where filename = '";
                      sql += fromfilename;
                      sql += "' and zonename = '";
                      sql += zonename;
                      sql +="' and output!=1";
                      qDebug()<<sql;
                      SQLHelper::insertRow(sql);
                  }else if(fromfilename.endsWith("msh"))
                  {
                      sql += "update cgns set type = '";
                      sql += property;
                      sql +="' where filename = '";
                      sql += fromfilename;
                      sql += "' and zonename = '";
                      sql += zonename;
                      sql +="' and output!=1";
                      SQLHelper::insertRow(sql);
                  }
                  ++it;
              }

          emit emit_signal_update();
               this->close();
      }else
      {
          this->close();
      }
}


void ModufyProDialog::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (column==1)
      {
          item->setFlags(item->flags()|Qt::ItemIsEditable);
      }
      else
      {
          item->setFlags(item->flags()&~(Qt::ItemIsEditable));
      }
}
