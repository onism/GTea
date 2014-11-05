#include "setprodialog.h"
#include "ui_setprodialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QAbstractButton>
#include <QMessageBox>
#include "sqlhelper.h"
SetProDialog::SetProDialog(QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::SetProDialog)
{
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList() << "name" << "property");
    file_count = 0;
}

SetProDialog::~SetProDialog()
{
    delete ui;
}


void SetProDialog::setItem(QList<QString> zone_names, QString filename, int filetype)
{
    ui->comboBox->clear();
    set_filename = filename;
    set_filetype = filetype;
    int n = zone_names.size();
    file_count = n;
    if( n > 0 ){

       for(int i = 0; i < n; i++){
           ui->comboBox->addItem(zone_names.at(i));
       }
    }else{
       QMessageBox::information(NULL, tr("sorry"), tr("You didn't have any zones."));
    }
}

void SetProDialog::on_pushButton_clicked()
{
    if(ui->radioButton->isChecked()){
        //solid
        QString select_name = ui->comboBox->currentText();
        QString property    = "solid";
        if(zone_set_pro_map.contains(select_name))
        {
            zone_set_pro_map.remove(select_name);
            zone_set_pro_map.insert(select_name,property);
            update_tree_widget();
        }else
        {
            zone_set_pro_map.insert(select_name,property);
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0, select_name);
            item->setText(1, property);
            ui->treeWidget->addTopLevelItem(item);
        }

    }else if(ui->radioButton_2->isChecked())
    {
        QString select_name = ui->comboBox->currentText();
        QString property    = "fluid";
        if(zone_set_pro_map.contains(select_name))
        {
            zone_set_pro_map.remove(select_name);
            zone_set_pro_map.insert(select_name,property);
            update_tree_widget();
        }else
        {
            zone_set_pro_map.insert(select_name,property);
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
            item->setText(0, select_name);
            item->setText(1, property);
            ui->treeWidget->addTopLevelItem(item);
        }
    }else
    {
         QMessageBox::information(NULL, tr("sorry"), tr("You must select one property."));
    }
}

void SetProDialog::on_buttonBox_clicked(QAbstractButton *button)
{
  if( zone_set_pro_map.size() != file_count )
  {
      QMessageBox::critical(NULL, "critical", "must set pro", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

      return;
  }
    if (QString::compare(button->text(),"OK", Qt::CaseInsensitive) == 0){
            //write db
         QString insertsql;
          QString wheresql;
        if(set_filetype == 1) //cgns file
        {
              insertsql = "update cgns set type='";
              wheresql  = " where filename ='"+set_filename+"' and zonename='";
        }else if(set_filetype == 2)
        {
              insertsql = "update msh set type='";
              wheresql  = " where filename ='"+set_filename+"' and facename='";
        }

        QMap<QString, QString>::const_iterator i;
        for (i = zone_set_pro_map.constBegin(); i != zone_set_pro_map.constEnd(); ++i) {
            QString sql = insertsql;
            sql += i.value();
            sql += "'";
            sql += wheresql;
            sql += i.key();
            sql += "' and output <> 1";
             qDebug()<<sql;
            //sqlhelper->update(sql);
             SQLHelper::update(sql);

        }
       // sqlhelper->closeSQLiteDB();

            accept();
            this->close();
        }
        else if (QString::compare(button->text(), "Cancel", Qt::CaseInsensitive) == 0){
            QMessageBox::critical(NULL, "critical", "must set pro", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            reject();
            return;

        }
        else{
        }
}

void SetProDialog::update_tree_widget()
{
    ui->treeWidget->clear();
    QMap<QString, QString>::const_iterator i;
    for (i = zone_set_pro_map.constBegin(); i != zone_set_pro_map.constEnd(); ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0,  i.key());
        item->setText(1, i.value());
        ui->treeWidget->addTopLevelItem(item);
    }


}
