#ifndef MODUFYPRODIALOG_H
#define MODUFYPRODIALOG_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class ModufyProDialog;
}

class ModufyProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModufyProDialog(QWidget *parent = 0);
    ~ModufyProDialog();
    void setItem(QString filename);
signals:
    void emit_signal_update();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::ModufyProDialog *ui;
    QString fromfilename;
};

#endif // MODUFYPRODIALOG_H
