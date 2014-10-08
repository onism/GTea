#ifndef GENERALSTR_H
#define GENERALSTR_H

#include <QDialog>

namespace Ui {
class GeneralStr;
}

class GeneralStr : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralStr(QWidget *parent = 0);
    ~GeneralStr();

private:
    Ui::GeneralStr *ui;
};

#endif // GENERALSTR_H
