#include "material.h"
#include "ui_material.h"

Material::Material(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Material)
{
    ui->setupUi(this);
}

Material::~Material()
{
    delete ui;
}
