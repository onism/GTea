#include "loaddlg.h"
#include "ui_loaddlg.h"

LoadDlg::LoadDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadDlg)
{
    ui->setupUi(this);
}

LoadDlg::~LoadDlg()
{
    delete ui;
}
