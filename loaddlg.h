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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LoadDlg *ui;
};

#endif // LOADDLG_H
