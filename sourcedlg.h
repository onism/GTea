#ifndef SOURCEDLG_H
#define SOURCEDLG_H

#include <QDialog>

namespace Ui {
class SourceDlg;
}

class SourceDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SourceDlg(QWidget *parent = 0);
    ~SourceDlg();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SourceDlg *ui;
};

#endif // SOURCEDLG_H
