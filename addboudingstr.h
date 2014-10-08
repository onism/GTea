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

private:
    Ui::AddBoudingStr *ui;
};

#endif // ADDBOUDINGSTR_H
