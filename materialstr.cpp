#include "materialstr.h"
#include "ui_materialstr.h"
#include "global.h"

MaterialStr::MaterialStr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialStr)
{
    ui->setupUi(this);

    ui->comboBox->setCurrentIndex(Global::material_str);
}

MaterialStr::~MaterialStr()
{
    delete ui;
}

void MaterialStr::on_pushButton_clicked()
{
    Global::material_str = ui->comboBox->currentIndex();

    accept();

}

void MaterialStr::on_pushButton_2_clicked()
{
    reject();
}
