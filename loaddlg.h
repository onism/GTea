#ifndef LOADDLG_H
#define LOADDLG_H

#include <QDialog>

namespace Ui {
class LoadDlg;
}

class LoadDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoadDlg(QWidget *parent = 0);
    ~LoadDlg();

private:
    Ui::LoadDlg *ui;
};

#endif // LOADDLG_H