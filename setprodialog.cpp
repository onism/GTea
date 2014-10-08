#include "setprodialog.h"
#include "ui_setprodialog.h"
#include <QMessageBox>
#include <QDebug>
#include <QAbstractButton>
SetProDialog::SetProDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetProDialog)
{
    ui->setupUi(this);
    ui->treeWidget->setHeaderLabels(QStringList() << "name" << "property");

}

SetProDialog::~SetProDialog()
{
    delete ui;
}


void SetProDialog::setItem(QList<QString> zone_names)
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
