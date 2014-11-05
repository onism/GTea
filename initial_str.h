#ifndef INITIAL_STR_H
#define INITIAL_STR_H

#include <QDialog>

namespace Ui {
class Initial_Str;
}

class Initial_Str : public QDialog
{
    Q_OBJECT

public:
    explicit Initial_Str(QWidget *parent = 0);
    ~Initial_Str();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Initial_Str *ui;
};

#endif // INITIAL_STR_H
