#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStackedWidget>
#include "3DView/AdapterWidget.h"
#include <QTreeWidget>
#include "sqlhelper.h"
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
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_action_2_triggered();

    void on_action_4_triggered();
    void del();
    void del_node();
    void on_treeWidget_customContextMenuRequested(const QPoint &pos);
    void on_loadtree_customContextMenuRequested(const QPoint &pos);
    void treeItemChanged(QTreeWidgetItem *item, int column);
    void openModel();
    void modify_color();

private:
    Ui::MainWindow *ui;
    QTreeWidget *load_tree;
    ViewerQT*	_glWidget;
    QAction* openModelAct;
    QList<QString> zone_names;
    QList<QString> base_name;
    QList<QString> reg_names;
    SQLHelper* SqlHelper;

};

#endif // MAINWINDOW_H
