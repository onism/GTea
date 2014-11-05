#ifndef ADDBOUDINGSTR_H
#define ADDBOUDINGSTR_H

#include <QDialog>

namespace Ui {
class AddBoudingStr;
}

class AddBoudingStr : public QDialog
{
    Q_OBJECT

public:
    explicit AddBoudingStr(QWidget *parent = 0);
    ~AddBoudingStr();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddBoudingStr *ui;
};

#endif // ADDBOUDINGSTR_H
