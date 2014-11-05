#ifndef MATERIALSTR_H
#define MATERIALSTR_H

#include <QDialog>

namespace Ui {
class MaterialStr;
}

class MaterialStr : public QDialog
{
    Q_OBJECT

public:
    explicit MaterialStr(QWidget *parent = 0);
    ~MaterialStr();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MaterialStr *ui;
};

#endif // MATERIALSTR_H
