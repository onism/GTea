#ifndef SETPRODIALOG_H
#define SETPRODIALOG_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <QAbstractButton>
namespace Ui {
class SetProDialog;
}

class SetProDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetProDialog(QWidget *parent = 0);
    ~SetProDialog();
    void setItem(QList<QString> zone_names);
private slots:
    void on_pushButton_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::SetProDialog *ui;
    QMap<QString,QString> zone_set_pro_map;

    void update_tree_widget();

};

#endif // SETPRODIALOG_H
