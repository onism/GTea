#ifndef DISPLAYFORM_H
#define DISPLAYFORM_H

#include <QDialog>

namespace Ui {
class DisPlayForm;
}

class DisPlayForm : public QDialog
{
    Q_OBJECT

public:
    explicit DisPlayForm(QWidget *parent = 0);
    ~DisPlayForm();

private:
    Ui::DisPlayForm *ui;
    void readInfo();
};

#endif // DISPLAYFORM_H
