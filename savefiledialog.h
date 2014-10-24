#ifndef SAVEFILEDIALOG_H
#define SAVEFILEDIALOG_H
#include <QMap>
#include <QDialog>
#include <QPair>
#include "changepositiondialog.h"
#include <QTreeWidgetItem>
#include "3DView/dbcgns.h"
namespace Ui {
class SaveFileDialog;
}

class SaveFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveFileDialog(QWidget *parent = 0);
    ~SaveFileDialog();
    void setchangedfile(QMap<int,ChangeObject> objects);

private slots:
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void save_file();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    void  showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos);
    QMap<QString,QMap<QString,ChangeObject>> filetree;
    Ui::SaveFileDialog *ui;
     DBCGNS* cgnsIO ;
    void construct_tree();
};

#endif // SAVEFILEDIALOG_H
