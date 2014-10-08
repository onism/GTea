#ifndef BOUNDINGSTR_H
#define BOUNDINGSTR_H

#include <QDialog>

namespace Ui {
class BoundingStr;
}

class BoundingStr : public QDialog
{
    Q_OBJECT

public:
    explicit BoundingStr(QWidget *parent = 0);
    ~BoundingStr();

private slots:
    void on_pushButton_clicked();

private:
    Ui::BoundingStr *ui;
};

#endif // BOUNDINGSTR_H
