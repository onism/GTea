#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include "3DView/AdapterWidget.h"
#include <QTreeWidget>
#include "sqlhelper.h"
#include "modufyprodialog.h"
#include <QCloseEvent>
#include <QMap>
#include "changepositiondialog.h"
namespace Ui {
class MainWindow;
}
//class Form;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void createActions();
    void createMenus();
    osg::ref_ptr<osg::Node> drawMash(int zone_number,int reg_number);
    osg::ref_ptr<osg::Node> drawSolution(int zone_number,int reg_number);
    void readSolutionCgns(char *file);
    osg::ref_ptr<osg::Node> drawaxis(osg::Vec3d center, double length);
    osgManipulator::Dragger* createDragger(const std::string& name);
    osg::Node* addDraggerToScene(osg::Node* scene, osgManipulator::CommandManager* cmdMgr, const std::string& name);
    osg::Node* createHUD();
    void closeEvent(QCloseEvent *event);
    QString checkfileesist(QString filepath);
    bool copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist);
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_action_2_triggered();

    void on_action_4_triggered();
    void del_node();
  //  void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_loadtree_customContextMenuRequested(const QPoint &pos);
    void treeItemChanged(QTreeWidgetItem *item, int column);
    void openModel();
    void changeDlg();
    void modify_color();

    void modify_pro();

    void updateBounding();

  //  void change_map_objects(QString filename,QString zonename,float x,float y,float z,float angle);
    void change_map_objects(ChangeObject object);


    void on_action_3_triggered();

private:
    Ui::MainWindow *ui;
    QTreeWidget *load_tree;
    ViewerQT*	_glWidget;
    QAction* openModelAct;
    QAction * change_postion_Act;
    QList<QString> zone_names;
    QList<QString> base_name;
    QList<QString> reg_names;
    SQLHelper* SqlHelper;
    ModufyProDialog * modify_dlg;
    QMap<int ,ChangeObject> change_objects;
    void add_map_objects(QString filename,QString zonename);
    ChangePositionDialog *change_postion_dlg;

};

#endif // MAINWINDOW_H
