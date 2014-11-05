#include "material.h"
#include "ui_material.h"
#include "global.h"

Material::Material(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Material)
{
    ui->setupUi(this);

    ui->comboBox->setCurrentIndex(Global::material_aco);
}

Material::~Material()
{
    delete ui;
}

void Material::on_pushButton_clicked()
{
    Global::material_aco = ui->comboBox->currentIndex();

    accept();

}

void Material::on_pushButton_2_clicked()
{
    reject();
}
