#ifndef CHANGEPOSITIONDIALOG_H
#define CHANGEPOSITIONDIALOG_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class ChangePositionDialog;
class ChangeObject;
}
class ChangeObject {
  //  Q_OBJECT
public:
  QString filename;
  QString zonename;
  float x_scale;
  float y_scale;
  float z_scale;
  float angle_x;
  float angle_y;
  float angle_z;
  float trans_x;
  float trans_y;
  float trans_z;
  ChangeObject::ChangeObject(QString filename,QString zonename);
  ChangeObject::ChangeObject(){}
};


class ChangePositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePositionDialog(QWidget *parent = 0);
    ~ChangePositionDialog();
    void setItem();
signals:
    void update_position(ChangeObject object);
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::ChangePositionDialog *ui;
};


#endif // CHANGEPOSITIONDIALOG_H
