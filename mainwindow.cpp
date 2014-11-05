#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPalette>
#include <QThread>
#include <QProcess>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QMessageBox>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgViewer/ViewerEventHandlers>
#include <QDebug>
#include <osg/ShapeDrawable>
#include <osgUtil/SmoothingVisitor>
#include <osg/PolygonMode>
#include <osg/ClipNode>
#include <osg/ClipPlane>
#include <osg/ComputeBoundsVisitor>
#include <osg/MatrixTransform>
#include <QTreeWidget>
#include <osg/Node>
#include <osg/BoundingSphere>
#include <osg/LineSegment>
#include <osgUtil/Optimizer>
#include <osg/Switch>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabPlaneDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/TrackballDragger>
#include <osgManipulator/Translate1DDragger>
#include <osgManipulator/Translate2DDragger>
#include <osgManipulator/TranslateAxisDragger>
#include "3DView/buildqttreeview.h"
#include "3DView/TreeViewItem.h"
#include "3DView/loadmsh.h"
#include "3DView/loadcgns.h"
#include "3DView/newdragger.h"
#include "3DView/InverseDist.h"
#include "3DView/BaseData.h"

#include "changepositiondialog.h"
//#include "dialog_test.h"

#include "initial_aco.h"
#include "initial_str.h"
#include "sourcedlg.h"
#include "addboundaco.h"
#include "addboudingstr.h"
#include "loaddlg.h"
#include "generalaco.h"
#include "generalstr.h"
#include "output.h"
#include "3DView/cgnslib.h"
#include "3DView/CGNSIO.h"
#include  "setprodialog.h"
#include "sqlhelper.h"
#include "material.h"

#include "modufyprodialog.h"
#include <QSqlQuery>
#include "coupling.h"
#include "materialstr.h"
#include "global.h"
//定义一些全局变量
QString x="lala";


#include "3DView/findnodevistor.h"
#include "modufyprodialog.h"

#include <QSqlQuery>
#include "3DView/removedata.h"
#include "savefiledialog.h"
#include "3DView/dbcgns.h"
#include "3DView/dbcgns_io.h"
#include "3DView/corelib.h"
#include "solutiondialog.h"

//#include "3DView/readcgnasolution.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


      createActions();
      createMenus();
      ui->treeWidget->expandAll();
ui->treeWidget->setMinimumWidth(300);
       ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
       ui->treeWidget->header()->setStretchLastSection(false);

      load_tree = new QTreeWidget;
      load_tree->setColumnCount(1);
      load_tree->setFixedSize(300,300);
      load_tree->setHeaderLabel("load file");
      QHBoxLayout *mainlayout = new QHBoxLayout;
      QVBoxLayout *vboxlayout = new QVBoxLayout;
      vboxlayout->addWidget(load_tree);
      vboxlayout->addWidget(ui->treeWidget);

      ui->treeWidget->expandAll();
      mainlayout->addLayout(vboxlayout);

      load_tree->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(load_tree, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(treeItemChanged(QTreeWidgetItem*,int)));
      connect(load_tree,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_loadtree_customContextMenuRequested(QPoint)));
      _glWidget = new ViewerQT;
      _glWidget->setMinimumSize(800, 550);
        // setCentralWidget(_glWidget);
      mainlayout->addWidget(_glWidget);
      QWidget *mwidget = new QWidget;
      mwidget->setLayout(mainlayout);
      setCentralWidget(mwidget);
     // SqlHelper = new SQLHelper();
        SQLHelper::openSQLiteDB();

        modify_dlg = new ModufyProDialog;
        change_postion_dlg = new ChangePositionDialog;
        qRegisterMetaType<ChangeObject>("ChangeObject");
      connect(modify_dlg,SIGNAL(emit_signal_update()),this,SLOT(updateBounding()));
      connect(change_postion_dlg,SIGNAL(update_position(ChangeObject)),this,SLOT(change_map_objects(ChangeObject)));
    //  connect(change_postion_dlg,SIGNAL(update_position(QString,QString,float,float,float,float)),this,SLOT(change_map_objects(QString,QString,float ,float,float,float)));

      file_count = 0;
      file_dim   = 0;

     clearSQL();//清空数据库表

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::treeItemChanged(QTreeWidgetItem *item_, int column)
{
    TreeViewItem *item = dynamic_cast<TreeViewItem*>(item_);

    if (item)
    {
        osg::ref_ptr<osg::Node> node = item->getOsgNode();

        if (node.valid())
        {
            // TODO: Create new command for setNodeMask
            if (item->checkState(0) == Qt::Unchecked){
                node->setNodeMask(0);
            }
            else
                node->setNodeMask(1);
//            if(node->className() == "Group" || strcmp(node->className(),"Group")){

//                _glWidget->ResetCameraPara();
//                _glWidget->update();
//                _glWidget->home();
//            }
        }

        for (int i=0; i<item->childCount(); ++i)
        {
            item->child(i)->setCheckState(0, item->checkState(0));
        }
    }
}
void MainWindow::createMenus()
{
    // create file menu
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openModelAct);
    fileMenu->addAction(change_postion_Act);
    fileMenu->addAction(save_Act);
    fileMenu->addAction(sdlg_Act);
//    fileMenu->addSeparator();
//    fileMenu->addAction(exitAct);

//    // create view menu
//    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
//    viewMenu->addAction(toolBarAct);
//    viewMenu->addAction(stateBarAct);

//    // create help menu
//    QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));
//    helpMenu->addAction(aboutAct);
}
void MainWindow::createActions()
{
    // file menu actions
    openModelAct = new QAction(/*QIcon("../images/open.png"),*/ tr("&Open Model"), this);
    openModelAct->setShortcut(QKeySequence::New);
    openModelAct->setStatusTip(tr("open a  model"));
    connect(openModelAct, SIGNAL(triggered()), this, SLOT(openModel()));

    change_postion_Act = new QAction(/*QIcon("../images/open.png"),*/ tr("&change position"), this);
    connect(change_postion_Act, SIGNAL(triggered()), this, SLOT(changeDlg()));

    save_Act = new QAction(/*QIcon("../images/open.png"),*/ tr("&save file"), this);
    connect(save_Act, SIGNAL(triggered()), this, SLOT(on_action_3_triggered()));

    sdlg_Act = new QAction(/*QIcon("../images/open.png"),*/ tr("&open solution file"), this);
    connect(sdlg_Act, SIGNAL(triggered()), this, SLOT(on_action_7_triggered()));

}

void MainWindow::changeDlg()
{
    change_postion_dlg->setItem();
   change_postion_dlg->exec();
}
void MainWindow::openModel()
{



     zone_names.clear();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),  ".", tr("  Files (*.cgns *.msh *.osg)"));

    fileName = checkfileesist(fileName);

    if(!fileName.isEmpty()){


         if(fileName.endsWith(".msh")){
             LoadMsh* loadmsh = new LoadMsh(fileName);

            if(!loadmsh->oroot)
                return;
            osg::ref_ptr<osgManipulator::Selection> selection = new osgManipulator::Selection;
            selection->addChild(loadmsh->oroot.get());
            selection->setName("selectionroot");
            _glWidget->AddModelNode(selection.get());


            // _glWidget->AddModelNode(loadmsh->oroot.get());
             _glWidget->ResetCameraPara();
             _glWidget->update();
             _glWidget->home();

             BuildQtTreeView visitor(loadmsh->oroot.get());
             loadmsh->oroot->accept(visitor);

                 //load_tree->clear();
             load_tree->addTopLevelItem(visitor.getRoot());
             load_tree->expandAll();
             load_tree->resizeColumnToContents(0);

             int face_size = loadmsh->face_wall_names.size();
            // SqlHelper->openSQLiteDB();
             QString insertsql = "insert into msh (filename,facename) values ('";
             for(int i=0; i < face_size; i++)
             {
                 QString sql = insertsql;
                 sql += fileName.section("/",-1);
                 sql += "','";
                 QString name = loadmsh->face_wall_names.at(i);

                 sql += name.section(" ",0,0);
                 sql += "')";
                 //SqlHelper->insertRow(sql);
                 SQLHelper::insertRow(sql);
                 zone_names.push_back(name.section(" ",0,0));
             }
             SqlHelper->closeSQLiteDB();
            SetProDialog setpro;
            setpro.setItem(zone_names,fileName.section("/",-1),2);
            if(setpro.exec() == QDialog::Accepted)
            {
                qDebug()<<"update";
                  updateBounding();
            }else
            {

            }

             return;
        }




          std::string filename = fileName.toStdString();
          char *cptr=new char[sizeof(char)*(filename.size()+1)];
          int i=0;
          for(i=0;i<filename.size();i++)
          {
                 cptr[i]=filename[i];
          }
          cptr[i]='\0';

            if( CGNSopen(cptr)) {
                if(file_count == 0){
                    file_dim = CellDim;

                    file_count++;

                }else if(file_dim != CellDim){
                    QMessageBox::critical(NULL, "critical", "维度不和", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

                    return;
                }else{
                    file_count++;

                }

               osg::ref_ptr< osg::Group > root = new osg::Group;

                root->setName(fileName.section("/",-1).toStdString());

                for(int i=0;i<1;i++)
                {
                     CGNSbase(i);
                     osg::ref_ptr< osg::Group > baseroot = new osg::Group;
                     baseroot->setName(BaseName);

                     for( int j = 0 ; j<  nzones;j++) {
                            CGNSzone(j);
                            osg::ref_ptr < osg::MatrixTransform > zoneroot = new osg::MatrixTransform ;

                           qDebug()<<zones[j].nnodes;
                           zoneroot->setName(zones[j].name);
                           QString name_temp = QString::fromLatin1(zones[j].name);
                           zone_names.push_back(name_temp);
                           add_map_objects(fileName.section("/",-1),name_temp);
                           int regs =  zones[j].nregs;
                           for(int k=0;k<regs;k++)
                           {
                              zoneroot->addChild(this->drawMash(j,k).get());

                              if(zones[j].regs[k].dim != CellDim)
                              {

                                  QString insertstring = "insert into cgns (filename,basename,zonename,regname,filepath,btype) values( '";

                                  insertstring += fileName.section("/",-1);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(BaseName);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(zones[j].name);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(zones[j].regs[k].name);
                                  insertstring += "','";
                                  insertstring +=  fileName;
                                  insertstring += "','";
                                  insertstring +=  "4";
                                  insertstring += "')";
                                //  SqlHelper->insertRow(insertstring);
                                  qDebug()<<insertstring;
                                  SQLHelper::insertRow(insertstring);
                                QString regname = QString::fromLatin1(zones[j].regs[k].name);
                                reg_names.push_back(regname);
                              }else {
//<<<<<<< HEAD
//                                  QString insertstring = "insert into cgns (filename,basename,zonename,regname,output) values( '";
//=======
                                  QString insertstring = "insert into cgns (filename,basename,zonename,regname,output,filepath) values( '";
//>>>>>>> origin/master
                                  insertstring += fileName.section("/",-1);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(BaseName);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(zones[j].name);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(zones[j].regs[k].name);
                                  insertstring += "','";
//<<<<<<< HEAD
//                                  insertstring += " 1')";
//                                //  SqlHelper->insertRow(insertstring);
//=======
                                  insertstring += "1";
                                  insertstring += "','";
                                  insertstring +=  fileName;
                                  insertstring += "')";
                                //  SqlHelper->insertRow(insertstring);
                                  qDebug()<<insertstring;
//>>>>>>> origin/master
                                  SQLHelper::insertRow(insertstring);
                              }
                               //zoneroot->addChild(this->drawSolution(j,k).get());
                            }//reg end
                      // baseroot->addChild(rot.get());
                             baseroot->addChild(zoneroot.get());
                     }//zones end
                     root->addChild(baseroot.get());
                }// base end



                osg::ref_ptr<osgManipulator::Selection> selection = new osgManipulator::Selection;
                selection->addChild(root.get() );
                selection->setName("selectionroot");
                _glWidget->AddModelNode(selection.get());

               // _glWidget->AddModelNode(root.get());
                _glWidget->ResetCameraPara();
                _glWidget->update();
                _glWidget->home();
                BuildQtTreeView visitor(root.get());
                root->accept(visitor);

                   // load_tree->clear();
                load_tree->addTopLevelItem(visitor.getRoot());
                load_tree->expandAll();
                load_tree->resizeColumnToContents(0);


                //set pro
                SetProDialog setpro;
                setpro.setItem(zone_names,fileName.section("/",-1),1);
                if(setpro.exec() == QDialog::Accepted)
                {   qDebug()<<"update";
                      updateBounding();
                }else
                {

                }

            }else{
                QMessageBox::critical(NULL, "critical", "can not open", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            }
            CGNSclose();
            delete cptr;
            cptr=NULL;

            // _glWidget->setSceneData(root.get());
         //    _glWidget->AddModelNode(root.get());

    }else{
      //  QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));

    }
        createTableFieldNames();

}
osg::ref_ptr<osg::Node> MainWindow::drawaxis(osg::Vec3d center, double length)
{
     osg::ref_ptr<osg::Geode> geode = new osg::Geode;
     geode->setName("axis");
     osg::ref_ptr<osg::Geometry> xgeom = new osg::Geometry();
     osg::ref_ptr<osg::Vec3Array> xv = new osg::Vec3Array;
     xgeom->setVertexArray(xv);
     osg::ref_ptr<osg::Vec4Array> xc = new osg::Vec4Array;
     xc->push_back(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
     xgeom->setColorArray(xc.get());
     xv->push_back(center);
     xv->push_back(osg::Vec3d(length,0,0)+center);
     xgeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
     geode->addDrawable(xgeom);
     osg::ref_ptr<osg::Geometry> ygeom = new osg::Geometry();
     osg::ref_ptr<osg::Vec3Array> yv = new osg::Vec3Array;
     ygeom->setVertexArray(yv);
     osg::ref_ptr<osg::Vec4Array> yc = new osg::Vec4Array;
     yc->push_back(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
     ygeom->setColorArray(yc.get());
     yv->push_back(center);
     yv->push_back(osg::Vec3d(0,length,0)+center);
     ygeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
     geode->addDrawable(ygeom);
     osg::ref_ptr<osg::Geometry> zgeom = new osg::Geometry();
     osg::ref_ptr<osg::Vec3Array> zv = new osg::Vec3Array;
     zgeom->setVertexArray(zv);
     osg::ref_ptr<osg::Vec4Array> zc = new osg::Vec4Array;
     zc->push_back(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
     zgeom->setColorArray(zc.get());
     zv->push_back(center);
     zv->push_back(osg::Vec3d(0,0,length)+center);
     zgeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
     geode->addDrawable(zgeom);
     return geode.get();

}
osg::ref_ptr<osg::Node> MainWindow::drawSolution(int zone_number, int reg_number)
{
   return NULL;
}

osg::ref_ptr<osg::Node> MainWindow::drawMash(int zone_number, int reg_number)
{
    Zone* zone = &zones[zone_number];
    Regn* r  = &zone->regs[reg_number];
    //osg::ref_ptr< osg::Group > root = new osg::Group;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setName(r->name);

    //定义颜色数组
    osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
    c->push_back(osg::Vec4(1.f,0.f,0.f,0.3f));
    //定义颜色数组
    osg::ref_ptr<osg::Vec4Array> b = new osg::Vec4Array;
    b->push_back(osg::Vec4(1.f,0.f,1.f,0.3f));

    if(r->dim == CellDim||reg_number==0){
        if (r->nedges) {

              int ne;
              for (ne = 0; ne < r->nedges; ne++) {

                    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
                    osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
                    geom->setVertexArray(v.get());
                    v->push_back(osg::Vec3(zone->nodes[r->edges[ne].nodes[0]][0],zone->nodes[r->edges[ne].nodes[0]][1],zone->nodes[r->edges[ne].nodes[0]][2]));
                    v->push_back(osg::Vec3(zone->nodes[r->edges[ne].nodes[1]][0],zone->nodes[r->edges[ne].nodes[1]][1],zone->nodes[r->edges[ne].nodes[1]][2]));

//                    geom->setColorArray(c.get());
//                     geom->setColorBinding(osg::Geometry::Bin);
                    osg::DrawElementsUInt* lineElems =  new osg::DrawElementsUInt(osg::PrimitiveSet::LINES, 0);
                    lineElems->push_back(0);
                    lineElems->push_back(1);
                    geom->addPrimitiveSet(lineElems);

                     geode->addDrawable(geom.get());

              }
            //  geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, r->nedges));
            //  geode->addDrawable(geom.get());

            }
    }else{
        osg::ref_ptr<osg::Geometry> trigeom = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> triv = new osg::Vec3Array;
        trigeom->setVertexArray(triv.get());

        osg::ref_ptr<osg::Geometry> quadgeom = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> quadv = new osg::Vec3Array;
        quadgeom->setVertexArray(quadv.get());

        osg::ref_ptr<osg::Geometry> linegeom = new osg::Geometry();
        osg::ref_ptr<osg::Vec3Array> linev = new osg::Vec3Array;
        linegeom->setVertexArray(linev.get());
    int nf,nn;
    Face *f;
    for (f = r->faces, nf = 0; nf < r->nfaces; nf++, f++) {

//        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

//        osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
//        geom->setVertexArray(v.get());
        switch (f->nnodes) {
        case 3:
            for (nn = 0; nn < f->nnodes; nn++){
                triv->push_back(osg::Vec3(zone->nodes[f->nodes[nn]][0],zone->nodes[f->nodes[nn]][1],zone->nodes[f->nodes[nn]][2]));
            }
            break;
        case 4:
            for (nn = 0; nn < f->nnodes; nn++){
                quadv->push_back(osg::Vec3(zone->nodes[f->nodes[nn]][0],zone->nodes[f->nodes[nn]][1],zone->nodes[f->nodes[nn]][2]));
            }
            break;
        case 2:
            for (nn = 0; nn < f->nnodes; nn++){
                linev->push_back(osg::Vec3(zone->nodes[f->nodes[nn]][0],zone->nodes[f->nodes[nn]][1],zone->nodes[f->nodes[nn]][2]));
            }
            break;
        default:
            break;
        }
//        for (nn = 0; nn < f->nnodes; nn++){
//            v->push_back(osg::Vec3(zone->nodes[f->nodes[nn]][0],zone->nodes[f->nodes[nn]][1],zone->nodes[f->nodes[nn]][2]));
//        }

//        geom->setColorArray(b.get());
//        geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
//        //设置关联方式
//        osg::Vec3Array* normals = new osg::Vec3Array;
//        normals->push_back(osg::Vec3(0, 0, 1));
//        geom->setNormalArray(normals);
//        geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
//         osgUtil::SmoothingVisitor::smooth( *geom );
//         geom->setUseDisplayList(true);
//         if(f->nnodes == 3)
//            geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, f->nnodes));
//         else if(f->nnodes == 4)
//            geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, f->nnodes));
//         else if(f->nnodes == 2)
//            geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, f->nnodes));
//         geode->addDrawable( geom.get());
    }



    if(triv->size()>0){
        trigeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,triv->size()));
        geode->addDrawable(trigeom.get());
    }
    if(quadv->size()>0){
        quadgeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,quadv->size()));
        geode->addDrawable(quadgeom.get());
    }
    if(linev->size()>0){
        linegeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,linev->size()*2));
        geode->addDrawable(linegeom.get());
    }


}
   osg::StateSet* ss = geode->getOrCreateStateSet();
   ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
   ss->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));


    return geode.release();
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parent = item->parent();

        if(parent!=NULL)
        {

        int row = parent->indexOfChild(item);
        Initial_Aco init_a;
        Initial_Str init_s;
        SourceDlg sourcedlg;
//        AddBoundAco addboundingaco;
//        AddBoudingStr addboundingstr;
        LoadDlg loaddlg;
        GeneralAco generalaco;
        GeneralStr generalstr;
        Output output;
        Material materialaco;
        MaterialStr materialstr;
        Coupling coupling;

            if(parent->text(0)=="acoustic"){

                    switch(row){
                    //acoustic的init对话框
                    case 3:
                        if(init_a.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
//                        updateBounding();
//<<<<<<< HEAD
//=======

//>>>>>>> origin/master
                     break;

                    //source对话框
                    case 0:
                        if(sourcedlg.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    //acoustic的general对话框
                    case 4:
                        if(generalaco.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    //acoustic的material对话框
                    case 1:
                        if(materialaco.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    default:
                        break;

                    }


            }

            if(parent->text(0)=="structure"){

                switch(row){
                    //structure的init对话框
                    case 2:
                        if(init_s.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    //load对话框
                    case 3:
                        if(loaddlg.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    //structure的general对话框
                    case 4:
                        if(generalstr.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    //material对话框
                    case 0:
                        if(materialstr.exec()==QDialog::Accepted)
                            item->setIcon(0,QIcon(":/new/images/on.png"));
                        break;

                    default:
                        break;

                }

            }

            //输出对话框和耦合边界对话框
            if(parent->text(0)=="ASI"){
                //output对话框
                if(row==3){
                    output.exec();
                }

                //耦合边界对话框
                if(row==2){
                    if(coupling.exec()==QDialog::Accepted){
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                        updateBounding();                               //设置耦合边界后更新树形菜单
                    }

                }

            }
        //设置边界
            if(parent->parent()!=NULL){
           //acoustic的设置边界对话框
           if(parent->parent()->text(0)=="acoustic"){
               Global::bounding = item;
               AddBoundAco addboundingaco;
               if(addboundingaco.exec()==QDialog::Accepted)
                   item->setIcon(0,QIcon(":/new/images/on.png"));
           }

           //structrue的设置边界对话框
           if(parent->parent()->text(0)=="structure"){
               Global::bounding = item;
               AddBoudingStr addboundingstr;
               if(addboundingstr.exec()==QDialog::Accepted)
                   item->setIcon(0,QIcon(":/new/images/on.png"));
           }

        }

       }



}







void MainWindow::on_action_4_triggered()
{
    close();
}

void MainWindow::on_action_2_triggered()
{

}


void MainWindow::on_loadtree_customContextMenuRequested(const QPoint &pos)
{
    if(load_tree->itemAt(pos)!=NULL){
    if(load_tree->itemAt(pos)->parent()==NULL){
        QMenu *pop_del_menu = new QMenu( );
//ModufyProDialog
        QAction *del_node_action = new QAction("del node",this);

        QAction *modify_node_action = new QAction(tr("modify pro"),this);

        connect(del_node_action,SIGNAL(triggered()),this,SLOT(del_node()));
        connect(modify_node_action,SIGNAL(triggered()),this,SLOT(modify_pro()));
        pop_del_menu->addAction(del_node_action);
        pop_del_menu->addAction(modify_node_action);
        pop_del_menu->addAction(openModelAct);

        auto globalPos = load_tree->mapToGlobal(pos);

        pop_del_menu->exec(globalPos);
       delete pop_del_menu;
    }else if(load_tree->itemAt(pos)->childCount()==0){
        QMenu *pop_color_menu = new QMenu( );

        QAction *color_action = new QAction("修改颜色",this);
        connect(color_action,SIGNAL(triggered()),this,SLOT(modify_color()));
        pop_color_menu->addAction(color_action);
        pop_color_menu->addAction(openModelAct);

        auto globalPos = load_tree->mapToGlobal(pos);

        pop_color_menu->exec(globalPos);
        delete pop_color_menu;
    }
    }else{
        QMenu *pop_open_menu = new QMenu( );
        pop_open_menu->addAction(openModelAct);
        auto globalPos = load_tree->mapToGlobal(pos);
        pop_open_menu->exec(globalPos);
        delete pop_open_menu;
    }
}

void MainWindow::modify_pro()
{

    QTreeWidgetItem *item_ = load_tree->currentItem();
    QString filename = item_->text(0);
    modify_dlg->clearItem();
    modify_dlg->setItem(filename.section("/",-1));
    modify_dlg->exec();

}

void MainWindow::del_node()
{
    QTreeWidgetItem *item_ = load_tree->currentItem();
    TreeViewItem *item = dynamic_cast<TreeViewItem*>(item_);

    if (item)
    {
        osg::ref_ptr<osg::Node> node = item->getOsgNode();
        RemoveData removevistor;
        node->accept(removevistor);
         _glWidget->ClearModelNodeByName(node);

         QString filename = item_->text(0);
         QString sql = "delete from  ";
         if(filename.endsWith("cgns")){
             sql += " cgns where filename = '";
             sql += filename;
             sql += "'";
             SQLHelper::deleteRow(sql);
             updateBounding();
             file_count--;
         }else if(filename.endsWith("msh")){
             sql += " msh where filename = '";
             sql += filename;
             sql += "'";
             SQLHelper::deleteRow(sql);
             updateBounding();
             file_count--;
         }

         qDebug()<<node->className();
         qDebug()<<node->asGroup()->getNumChildren();
       //  QString sql = "delete from cgns where filename ='";
         //QString filename =
       // _glWidget->m_rpSceneGroupRoot->removeChild(node);

        _glWidget->ResetCameraPara();
        _glWidget->update();
        _glWidget->home();
    }
    delete item_;
}

void MainWindow::modify_color()
{
    QTreeWidgetItem *item_ = load_tree->currentItem();
    QTreeWidgetItem * parent_ = item_->parent();
    TreeViewItem *item = dynamic_cast<TreeViewItem*>(item_);
    TreeViewItem *parent = dynamic_cast<TreeViewItem*>(parent_);
    if (item)
    {
        osg::ref_ptr<osg::Node> node = item->getOsgNode();
        osg::ref_ptr<osg::Node> newnode = item->getOsgNode();
        osg::ref_ptr<osg::Group> parent_node = dynamic_cast<osg::Group*>(parent->getOsgNode());
        QColor color  = QColorDialog::getColor(Qt::green, this);
        if (color.isValid()) {
            osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
            c->push_back(osg::Vec4d(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
            //  qDebug()<<color.name()<<color.alphaF()<<color.redF()<<color.greenF()<<color.blueF();
              for (unsigned int i=0; i<newnode->asGeode()->getNumDrawables(); ++i)
              {
                    osg::ref_ptr<osg::Drawable> drawable = newnode->asGeode()->getDrawable(i);

                    if (drawable.valid())
                    {
                        osg::ref_ptr<osg::Geometry> geometry = drawable->asGeometry();

                        if (geometry.valid())
                        {
                            geometry->setColorArray(c.get());
                            geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
                        }
                    }
                }
              parent_node->replaceChild(node,newnode);

       }
    }

}


void MainWindow::readSolutionCgns(char* file)
{


       return ;

}

osgManipulator::Dragger* MainWindow::createDragger(const std::string& name)
{
    osgManipulator::Dragger* dragger = 0;
       if ("TabPlaneDragger" == name)
       {
           osgManipulator::TabPlaneDragger* d = new osgManipulator::TabPlaneDragger();
           d->setupDefaultGeometry();
           dragger = d;
       }
       else if ("TabPlaneTrackballDragger" == name)
       {
           osgManipulator::TabPlaneTrackballDragger* d = new osgManipulator::TabPlaneTrackballDragger();
           d->setupDefaultGeometry();
           dragger = d;
       }
       else if ("TrackballDragger" == name)
       {
           osgManipulator::TrackballDragger* d = new osgManipulator::TrackballDragger();
           d->setupDefaultGeometry();
           dragger = d;
       }
       else if ("Translate1DDragger" == name)
       {
           osgManipulator::Translate1DDragger* d = new osgManipulator::Translate1DDragger();
           d->setupDefaultGeometry();
           dragger = d;
       }
       else if ("Translate2DDragger" == name)
       {
           osgManipulator::Translate2DDragger* d = new osgManipulator::Translate2DDragger();
           d->setupDefaultGeometry();
           dragger = d;
       }
       else if ("TranslateAxisDragger" == name)
       {
           osgManipulator::TranslateAxisDragger* d = new osgManipulator::TranslateAxisDragger();
            d->setupDefaultGeometry();
           dragger = d;
       }
       else
       {
           osgManipulator::TabBoxDragger* d = new osgManipulator::TabBoxDragger();
           d->setupDefaultGeometry();
           dragger = d;
       }



       return dragger;
}


osg::Node* MainWindow::addDraggerToScene(osg::Node* scene, osgManipulator::CommandManager* cmdMgr, const std::string& name)
{
       scene->getOrCreateStateSet()->setMode(GL_NORMALIZE, osg::StateAttribute::ON);

       osgManipulator::Selection* selection = new osgManipulator::Selection;
       selection->addChild(scene);

       osgManipulator::Dragger* dragger = createDragger(name);

       osg::Group* root = new osg::Group;
       root->addChild(dragger);
       root->addChild(selection);
       root->addChild(createHUD());

       float scale = scene->getBound().radius() * 1.6;
       dragger->setMatrix(osg::Matrix::scale(scale, scale, scale) *
                          osg::Matrix::translate(scene->getBound().center()));
       cmdMgr->connect(*dragger, *selection);

       return root;
}


osg::Node* MainWindow::createHUD()
{
    osg::Geode* geode = new osg::Geode();

    std::string timesFont("fonts/arial.ttf");

    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    osgText::Text* text = new  osgText::Text;
    geode->addDrawable( text );

    osg::Vec3 position(50.0f,50.0f,0.0f);
    text->setPosition(position);
    text->setText("Use the Tab key to switch between the trackball and pick modes.");
    text->setFont(timesFont);

    osg::Camera* camera = new osg::Camera;

    // set the projection matrix
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,1024));

    // set the view matrix
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera->setRenderOrder(osg::Camera::POST_RENDER);

    camera->addChild(geode);

    return camera;
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QString del_cgns = "delete from cgns";
    QString del_msh  = "delete from msh";
    //SqlHelper->openSQLiteDB();
   // SqlHelper->deleteRow(del_cgns);
   // SqlHelper->deleteRow(del_msh);
   // SqlHelper->closeSQLiteDB();
    SQLHelper::deleteRow(del_cgns);
    SQLHelper::deleteRow(del_msh);
    event->accept();

}


QString MainWindow::checkfileesist(QString filepath)
{
    //return filename,the filename should change filepath ,if file  already exist then rename else no change return filename
    //1.get the filename
    QString file_name = filepath.section("/",-1);

    QString file_suffix = file_name.section(".",1,-1);
    qDebug()<<file_suffix;
    QString real_name = file_name.section(".",0,0);
   // qDebug()<<real_name;
    QString sql="";
    if(file_name.endsWith("cgns")){
         sql = "select distinct  filename from cgns";
    }else if(file_name.endsWith("msh")){
         sql = "select distinct  filename from msh";
    }

    QList<QString> namelist = SQLHelper::selectbysql(sql);
    int n = namelist.size();
    if(n>0)
    {
        for(int i=0;i<n;i++)
        {
            QString temp = namelist.at(i).section(".",0,0);
            qDebug()<<temp<<temp.compare(real_name);
            if(temp.compare(real_name) == 0){
                QString tip = "the file is already exist,\n if you want to load we will change the name from ";
                tip += real_name;
                tip += " to ";
                tip +=real_name;
                tip += "1";
                QMessageBox::StandardButton rb = QMessageBox::warning(NULL,"warning",tip,QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if(rb == QMessageBox::Yes)
                {
                    QString base_file_name = filepath.section(".",0,0);
                    QString newfilepath = base_file_name.replace(real_name,real_name+"1").append(".")+file_suffix;
//qDebug()<<newfilepath<<filepath;
                   // QFile::rename(filepath, newfilepath);
//                    qDebug()<<QFile::rename(filepath, newfilepath);
                    copyFileToPath(filepath,newfilepath,true);
                    return newfilepath;
                }else{
                    return "";
                }

            }
        }
        return filepath;
    }else
    {
        return filepath;
    }

}

bool MainWindow::copyFileToPath(QString sourceDir ,QString toDir, bool coverFileIfExist)
{
        if (sourceDir == toDir){
            return true;
        }
        if (!QFile::exists(sourceDir)){
            return false;
        }
        QDir *createfile     = new QDir;
        bool exist = createfile->exists(toDir);
        if (exist){
            if(coverFileIfExist){
                createfile->remove(toDir);
            }
        }//end if

        if(!QFile::copy(sourceDir, toDir))
        {
            return false;
        }
        return true;
}
//<<<<<<< HEAD
//void MainWindow::updateBounding(){
//    //首先清空两个bounding下的item
//=======

void MainWindow::updateBounding(){
    //首先清空两个bounding下的item
        QList< QTreeWidgetItem *> templist= ui->treeWidget->findItems("bounding",Qt::MatchRecursive,0);
        foreach (QTreeWidgetItem *temp, templist) {
            QList<QTreeWidgetItem *> temp2=temp->takeChildren();
            foreach(QTreeWidgetItem *temp3 , temp2){
                delete temp3;
            }
        }

        QStringList listAco,listStr;
        QSqlQuery query(SQLHelper::dbLink);
        //acoustic的边界list
        query.exec("select filename,zonename,regname from cgns where type='fluid' and btype<>6");
        while(query.next()){
            listAco<<query.value("filename").toString()+QString("--")\
                     +query.value("zonename").toString()+QString("--")\
                     +query.value("regname").toString();
    //        qDebug()<<listAco;
        }
        //structure的边界list
        query.exec("select filename,zonename,regname from cgns where type='solid' and btype<>6");
        while(query.next()){
            listStr<<query.value("filename").toString()+QString("--")\
                     +query.value("zonename").toString()+QString("--")\
                     +query.value("regname").toString();
    //        qDebug()<<listStr;
        }

        QTreeWidgetItem *parentAco = ui->treeWidget->topLevelItem(0)->child(0)->child(2);//acoustc的bounding节点
        QTreeWidgetItem *parentStr = ui->treeWidget->topLevelItem(0)->child(1)->child(1);//structure的bounding节点



        //构造acoustc的bounding孩子节点
        if(!listAco.isEmpty()){


            foreach (QString tmp, listAco){


                query.exec(QString("select flag from cgns where filename='%1' and zonename='%2' and regname='%3'")\
                           .arg(tmp.section("--",0,0)).arg(tmp.section("--",1,1)).arg(tmp.section("--",2,2)));

                while(query.next()){
                            if(query.value("flag").toInt()==1){
                                (new QTreeWidgetItem(parentAco,QStringList(tmp)))->setIcon(0,QIcon(":/new/images/on.png"));
                            }
                            else{
                                (new QTreeWidgetItem(parentAco,QStringList(tmp)))->setIcon(0,QIcon(":/new/images/off.png"));
                            }
                }

            }
         }
        //构造structure的bounding孩子节点
        if(!listStr.isEmpty()){
            foreach (QString tmp, listStr) {
                query.exec(QString("select flag from cgns where filename='%1' and zonename='%2' and regname='%3'")\
                           .arg(tmp.section("--",0,0)).arg(tmp.section("--",1,1)).arg(tmp.section("--",2,2)));
                while(query.next()){
                    if(query.value("flag").toInt()==1){
                        (new QTreeWidgetItem(parentStr,QStringList(tmp)))->setIcon(0,QIcon(":/new/images/on.png"));
                    }
                    else{
                        (new QTreeWidgetItem(parentStr,QStringList(tmp)))->setIcon(0,QIcon(":/new/images/off.png"));
                    }
                }
            }

        }




}
//=======





void MainWindow::add_map_objects(QString filename, QString zonename)
{
    ChangeObject object(filename,zonename);
    int n = change_objects.size();
    change_objects.insert(n+1,object);

}

void MainWindow::change_map_objects(ChangeObject object)
{
  //1.get the zone which need matrix

    //float angle = object.angle_scale;
    QString filename = object.filename;
    QString zonename = object.zonename;
    ChangeObject object_map;
    // if the key < 0 ,then the file is changed .need save another file
    QMap<int, ChangeObject>::const_iterator map_i;
    for (map_i = change_objects.constBegin(); map_i != change_objects.constEnd(); ++map_i) {
        object_map = map_i.value();
        if(object_map.filename == filename && object_map.zonename == zonename){
            object_map.angle_x += object.angle_x;
            object_map.angle_y += object.angle_y;
            object_map.angle_z += object.angle_z;
            object_map.trans_x += object.trans_x;
            object_map.trans_y += object.trans_y;
            object_map.trans_z += object.trans_z;
            object_map.x_scale *= object.x_scale;
            object_map.y_scale *= object.y_scale;
            object_map.z_scale *= object.z_scale;

            break;

        }

    }
    int key = map_i.key();
    change_objects.remove(key);
    if(key < 0){
        change_objects.insert(key,object_map);
    }else{
        key = 0-key;
        change_objects.insert(key,object_map);
    }


   // qDebug()<<x<<y<<z<<angle<<filename<<zonename;
    //2 find the node based on the filename and zonename
    FindNodeVistor node_finder(zonename.toStdString(),filename.toStdString());
    _glWidget->m_rpSceneGroupRoot->accept(node_finder);
    if (!node_finder._foundNodes.empty()) {
       osg::ref_ptr<osg::Node> obj = node_finder._foundNodes.front().get();

       osg::Matrixd transform= obj->asTransform()->asMatrixTransform()->getMatrix();

//      transform.setTrans(osg::Vec3d(x,y,z));
//      transform.setRotate(osg::Matrix::translate(object.trans_x, object.trans_y, object.trans_z) *
//                          osg::Matrix::rotate(osg::DegreesToRadians(-90.0f), 0, 0, 1) *
//                          osg::Matrix::scale(object.x_scale,object.y_scale,object.z_scale));
       if(object.trans_x ==0 && object.trans_y == 0 && object.trans_z ==0){
           obj->asTransform()->asMatrixTransform()->setMatrix(
                                                               osg::Matrix::rotate(osg::DegreesToRadians(object.angle_x), 1, 0, 0) *
                                                               osg::Matrix::rotate(osg::DegreesToRadians(object.angle_y), 0, 1, 0) *
                                                               osg::Matrix::rotate(osg::DegreesToRadians(object.angle_z), 0, 0, 1) *
                                                               osg::Matrix::scale(object.x_scale,object.y_scale,object.z_scale) );
       }else{
           obj->asTransform()->asMatrixTransform()->setMatrix( osg::Matrix::translate(object.trans_x, object.trans_y, object.trans_z) *
                                                               osg::Matrix::rotate(osg::DegreesToRadians(object.angle_x), 1, 0, 0) *
                                                               osg::Matrix::rotate(osg::DegreesToRadians(object.angle_y), 0, 1, 0) *
                                                               osg::Matrix::rotate(osg::DegreesToRadians(object.angle_z), 0, 0, 1) *
                                                               osg::Matrix::scale(object.x_scale,object.y_scale,object.z_scale) );
       }

//       qDebug()<<obj->className();
//       osg::Matrixd transform=obj->asTransform()->asMatrixTransform()->getMatrix();
//          qDebug()<<obj->className();
//       transform.setTrans(osg::Vec3d(x,y,z));
//       obj->asTransform()->asMatrixTransform()->setMatrix( transform );
//       qDebug()<<obj->getParent(0)->className();
//       if (dynamic_cast<osg::MatrixTransform*>(obj->getParent(0)))
//       {
//           osg::Matrixd transform= obj->getParent(0)->asTransform()->asMatrixTransform()->getMatrix();
//           transform.setTrans(osg::Vec3d(x,y,z));
//           obj->getParent(0)->asTransform()->asMatrixTransform()->setMatrix( transform );
//             qDebug()<<obj->getParent(0)->className();
//       }
//       else
//       {

//           osg::ref_ptr < osg::MatrixTransform > rot = new osg::MatrixTransform ;
//           rot ->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(45.0), 1, 0, 0)*osg::Matrix::scale(1.5,1.5,
//           0.5)*osg::Matrix::translate(40, 0, -2)) ;
//           rot ->addChild(obj.get()) ;
//           qDebug()<<obj->getParent(0)->className()<<obj->getParent(0)->getNumChildren();
//           obj->getParent(0)->replaceChild(obj,rot);
//       }
//       if(obj->getParent(0)->className() == "Group"){
//       osg::ref_ptr < osg::MatrixTransform > rot = new osg::MatrixTransform ;
//       rot ->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(45.0), 1, 0, 0)*osg::Matrix::scale(1.5,1.5,
//       0.5)*osg::Matrix::translate(40, 0, -2)) ;
//       rot ->addChild(obj.get()) ;
//       qDebug()<<obj->getParent(0)->className()<<obj->getParent(0)->getNumChildren();
//       obj->getParent(0)->replaceChild(obj,rot);
//       }else if(obj->getParent(0)->className() == "MatrixTransform")
//       {
//          osg::Matrixd transform= obj->getParent(0)->asTransform()->asMatrixTransform()->getMatrix();
//          transform.setTrans(osg::Vec3d(x,y,z));
//          obj->getParent(0)->asTransform()->asMatrixTransform()->setMatrix( transform );
//       }
       //obj->asTransform()->asMatrixTransform()->setMatrix(osg::Matrix::translate(x,y,z));
    }

}

void MainWindow::on_action_3_triggered()
{
  SaveFileDialog savedlg;
  savedlg.setchangedfile(change_objects);
  savedlg.exec();

}

void MainWindow::on_action_7_triggered()
{
    SolutionDialog sdlg;
    sdlg.exec();
}
//>>>>>>> origin/master

//生成数据库FieldNames
void MainWindow::createTableFieldNames()
{
    //连接数据库
 //   QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
 //   QString dbPath(QDir::current().path());
//    dbPath.append(QDir::separator()).append("db").append(QDir::separator()).append("db.sqlite");
//    dbPath = QDir::toNativeSeparators(dbPath);
//    db.setDatabaseName(dbPath);
//    if(!db.open())return;  //连接不成功返回

    QSqlQuery queryCgns(SQLHelper::dbLink);
    queryCgns.exec(QString("select filename,regname,id from cgns where output=1"));  //读取数据库目标集

    QSqlQuery queryFieldName;
    queryFieldName.clear();
    queryFieldName.prepare("insert into FieldNameLocation(ID,Name,X,Y,Z) values(:ID,:Name,:X,:Y,:Z)");

    while(queryCgns.next())
    {
        QString str1=queryCgns.value(0).toString();
        QString str2=queryCgns.value(1).toString();
        QString fieldNameStr="";
        fieldNameStr.append(str1);
        fieldNameStr.append("_");
        fieldNameStr.append(str2);
        qDebug()<<fieldNameStr;

        queryFieldName.bindValue(":ID",queryCgns.value(2).toString().toInt());
        queryFieldName.bindValue(":Name",fieldNameStr);
        queryFieldName.bindValue(":X",0);
        queryFieldName.bindValue(":Y",0);
        queryFieldName.bindValue(":Z",0);
        queryFieldName.exec();   //插入
    }
  // db.close();
}

//清空数据库
void MainWindow::clearSQL()
{
    QString del_cgns = "delete from cgns";
    QString del_msh  = "delete from msh";
    QString del_FieldName="delete from FieldNameLocation";
    SQLHelper::deleteRow(del_cgns);
    SQLHelper::deleteRow(del_msh);
    SQLHelper::deleteRow(del_FieldName);  //清空数据库表

}
