#include "addboundaco.h"
#include "ui_addboundaco.h"

AddBoundAco::AddBoundAco(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBoundAco)
{
    ui->setupUi(this);


}

//void AddBoundAco::setItem(QList<QString> structure)
//{
//    ui->comboBox_2->clear();
//    int n = structure.size();
//    if( n > 0 ){
//       for(int i = 0; i < n; i++){
//           ui->comboBox_2->addItem(structure.at(i));
//       }
//    }else{

//    }
//}
//void AddBoundAco::getcurrenttex()
//{
//    str = ui->comboBox_2->currentText();
//}
AddBoundAco::~AddBoundAco()
{
    delete ui;
}



//void AddBoundAco::on_buttonBox_clicked(QAbstractButton *button)
//{
//    getcurrenttex();
//}
