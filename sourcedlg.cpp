#include "sourcedlg.h"
#include "ui_sourcedlg.h"

SourceDlg::SourceDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SourceDlg)
{
    ui->setupUi(this);


}

SourceDlg::~SourceDlg()
{
    delete ui;
}



