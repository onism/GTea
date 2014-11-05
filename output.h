#ifndef OUTPUT_H
#define OUTPUT_H

#include <QDialog>
#include "QSqlQuery"
#include "sqlhelper.h"

namespace Ui {
class Output;
}

class Output : public QDialog
{
    Q_OBJECT

public:
    explicit Output(QWidget *parent = 0);
    ~Output();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::Output *ui;
    void InitComboBox();//初始化ComboBox

    void SaveFile();//保存文件
    void WriteTxt(QString fileName);//写入文本文件
    bool copyFile(QString sourceDir ,QString toDir, bool coverFileIfExist);//复制文件
};

#endif // OUTPUT_H
