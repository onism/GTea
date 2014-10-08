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


private:
    Ui::SourceDlg *ui;
};

#endif // SOURCEDLG_H
