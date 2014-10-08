#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPalette>
#include "dialog.h"
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
#include "dialog_test.h"
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
//#include "3DView/readcgnasolution.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    ui->treeWidget->setVisible(false);
//    ui->treeWidget_2->setVisible(false);
//    ui->label_15->setVisible(false);
//    ui->label->setVisible(false);
//    ui->label_15->setPixmap(QPixmap(":/new/images/3dpic.png"));
      createActions();
      createMenus();
      ui->treeWidget->expandAll();
      ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

      load_tree = new QTreeWidget;
      load_tree->setColumnCount(1);
      load_tree->setHeaderLabel("导入网格文件");
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
      SqlHelper = new SQLHelper();
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

}
void MainWindow::openModel()
{



//    int nDiameter = 50;
//    TInverseDist* pInterpolater = new TInverseDist(50, 4);
//    std::vector<Point3D> input;
//    input.push_back(Point3D(1,1,2));
//    input.push_back(Point3D(2,2,3));
//    input.push_back(Point3D(3,3,4));
//    input.push_back(Point3D(4,4,4));
//    input.push_back(Point3D(5,5,5));
//    input.push_back(Point3D(1,2,2));
//    input.push_back(Point3D(2,1,6));
//    input.push_back(Point3D(3,1,2));
//    input.push_back(Point3D(6,1,8));
//    input.push_back(Point3D(10,10,12));
//    input.push_back(Point3D(12,15,20));
//    vector<double> vecZs;
////qDebug()<<"begin Interpolater";
//    for(int j=0; j<=nDiameter; j++) {
//        for(int i=0; i<=nDiameter; i++) {
//          //  qDebug()<<i<<j;
//            double z = pInterpolater->GetInterpolatedZ(j - nDiameter/2, i - 0.5 * nDiameter, input);
//            vecZs.push_back(z);
//        }
//    }

//    delete pInterpolater;
//    vector<double>::iterator iter2;
//    iter2 = max_element(vecZs.begin(), vecZs.end());
//    double m_dThickMax = *iter2;
//    iter2 = min_element(vecZs.begin(), vecZs.end());
//    double m_dThickMin = *iter2;
//    //qDebug()<<"max"<<m_dThickMax<<"min"<<m_dThickMin;
//    vector<double> value_z;
//    double MinMaxDiff = m_dThickMax - m_dThickMin;
//    double widthRatio = nDiameter;
//    double ratio = MinMaxDiff/widthRatio * 4;
//    for(int i=0;i<vecZs.size();i++)
//    {
//        value_z.push_back((vecZs[i]-m_dThickMin)/ratio);
//    }

//    double m_dXrange = nDiameter + 1;
//    double m_dYrange = (m_dThickMax-m_dThickMin)/ratio;
//    double m_dZrange = nDiameter;
//    osg::ref_ptr<osg::Geode> node = new osg::Geode;
//    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//    osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;
//    osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array;
//    geom->setVertexArray(vert);
//    geom->setColorArray(color);
//    for(int j=0;j<m_dZrange-1;++j)
//    {
//        //qDebug()<<j<<m_dXrange*j<<value_z.size();
//       int i=0;
//       int xpos = i;
//       int ypos = j;
//       double thickness = value_z[(int)(i+m_dXrange*j)];
//       double x = -m_dXrange/2 + xpos;
//       double y = -m_dYrange/2 + thickness;
//       double z = -m_dZrange/2 + ypos;
//       vert->push_back(osg::Vec3(x,y,z));
//     //  color->push_back(SetDataColor(thickness,m_dYrange,m_dThickMin ));
//       for(i=0; i<m_dXrange;i++)
//       {
//         //   qDebug()<<i<<i+m_dXrange*j<<value_z.size()<<value_z[(int)(i+m_dXrange*j)];
//           xpos = i;
//           ypos = j+1;
//             thickness = value_z[(int)(i+m_dXrange*j)];
//            x = -m_dXrange/2 + xpos;
//             y = -m_dYrange/2 + thickness;
//             z = -m_dZrange/2 + ypos;
//           vert->push_back(osg::Vec3(x,y,z));
//          // qDebug()<<"vert";
//          // color->push_back(SetDataColor(thickness,m_dYrange ,m_dThickMin));
//          // qDebug()<<"color";
//           xpos = i+1;
//           ypos = j;
//          //  qDebug()<<i<<i+m_dXrange*j<<value_z.size()<<"2";
//             thickness = value_z[(int)(i+m_dXrange*j)];

//             x = -m_dXrange/2 + xpos;
//             y = -m_dYrange/2 + thickness;
//             z = -m_dZrange/2 + ypos;
//           vert->push_back(osg::Vec3(x,y,z));
//         //  color->push_back(SetDataColor(thickness,m_dYrange,m_dThickMin));

//       }

//    }
//qDebug()<<vecZs.size();
//    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,0,vert->size()));
//    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
//    node->addDrawable(geom.get());

//    _glWidget->AddModelNode(node.get());
//qDebug()<<vecZs.size();

//    // _glWidget->AddModelNode(loadmsh->oroot.get());
//     _glWidget->ResetCameraPara();
//     _glWidget->update();
//     _glWidget->home();

//qDebug()<<vecZs.size();





zone_names.clear();








    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),  ".", tr("  Files (*.cgns *.msh *.osg)"));

//     osg::Node* axes = osgDB::readNodeFile(fileName.toLatin1().data());

//     _glWidget->AddModelNode(axes);
//     _glWidget->ResetCameraPara();
//     _glWidget->update();
//     _glWidget->home();
    if(!fileName.isEmpty()){
        osg::ref_ptr<osgManipulator::CommandManager> commandManager =
                new osgManipulator::CommandManager();

         if(fileName.endsWith(".msh")){
             LoadMsh* loadmsh = new LoadMsh(fileName);
         //    _glWidget->ClearModelNode();

            // osg::BoundingSphere boundingSphere = loadmsh->oroot->getBound();
            // osg::Vec3d center = boundingSphere.center();
            // double length = boundingSphere.radius();
            // loadmsh->oroot->addChild(drawaxis(center,length));


//             osgUtil::Optimizer optimizer;
//             optimizer.optimize(loadmsh->oroot.get());

             //add dragger
            if(!loadmsh->oroot)
                return;
//            osg::StateSet * state = loadmsh->oroot->getOrCreateStateSet();
//            state->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
//            osg::ref_ptr<osg::Switch> switchnode = new osg::Switch;
//            osg::ref_ptr<NewDragger> dragger = new NewDragger;
//            float scale = loadmsh->oroot->getBound().radius()*1.4;
//            dragger->setMatrix(osg::Matrix::scale(scale,scale,scale)*osg::Matrix::translate(loadmsh->oroot->getBound().center()));
//            dragger->setupDefaultGeometry();

//            osg::ref_ptr<osgManipulator::Selection> selection = new osgManipulator::Selection;
//            selection->addChild(loadmsh->oroot.get());
//            osg::Vec3 trans = osg::Vec3(0.0,0.0,0.0);
//            osg::Quat qu = osg::Quat(osg::DegreesToRadians(0.0),osg::Vec3(1.0,0.0,0.0));
//            selection->setMatrix(osg::Matrix::translate(trans)*osg::Matrix::rotate(qu));
//            switchnode->addChild(dragger.get());
//            switchnode->addChild(selection.get());
//            _glWidget->AddModelNode(switchnode.get());
//            commandManager->connect(*dragger.get(),*selection.get());
            osg::ref_ptr<osg::MatrixTransform> transform_1 = new osg::MatrixTransform;
            transform_1.get()->addChild(addDraggerToScene(loadmsh->oroot.get(),commandManager,"TranslateAxisDragger"));

            _glWidget->AddModelNode(transform_1.get());


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

             int face_size = loadmsh->Face_Names.size();
             for(int i=0; i < face_size; i++)
             {
                 QString tmp =QString::fromLatin1(loadmsh->Face_Names.at(i).name);
                 zone_names.push_back(tmp);
             }

            SetProDialog setpro;
            setpro.setItem(zone_names);
            if(setpro.exec() == QDialog::Accepted)
            {

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
  //qDebug()<<cptr;
//readSolutionCgns(cptr);
            if( CGNSopen(cptr)) {
               osg::ref_ptr< osg::Group > root = new osg::Group;
               root->setName(cptr);
               SqlHelper->openSQLiteDB();

                for(int i=0;i<1;i++)
                {
                     CGNSbase(i);
                     osg::ref_ptr< osg::Group > baseroot = new osg::Group;
                     baseroot->setName(BaseName);

                     for( int j = 0 ; j<  nzones;j++) {
                            CGNSzone(j);
                           osg::ref_ptr< osg::Group > zoneroot = new osg::Group;
                           qDebug()<<zones[j].nnodes;
                           zoneroot->setName(zones[j].name);
                           QString name_temp = QString::fromLatin1(zones[j].name);
                           zone_names.push_back(name_temp);
                           int regs =  zones[j].nregs;
                           for(int k=0;k<regs;k++)
                           {
                              zoneroot->addChild(this->drawMash(j,k).get());

                              if(zones[j].regs[k].dim != CellDim)
                              {
                                  QString insertstring = "insert into cgns (filename,basename,zonename,regname) values( '";
                                  insertstring += fileName;
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(BaseName);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(zones[j].name);
                                  insertstring += "','";
                                  insertstring +=  QString::fromLatin1(zones[j].regs[k].name);
                                  insertstring += "')";
                                  SqlHelper->insertRow(insertstring);
                                QString regname = QString::fromLatin1(zones[j].regs[k].name);
                                reg_names.push_back(regname);
                              }
                               //zoneroot->addChild(this->drawSolution(j,k).get());
                            }//reg end

                            baseroot->addChild(zoneroot.get());
                     }//zones end
                     root->addChild(baseroot.get());
                }// base end

              //  osg::BoundingSphere boundingSphere = root->getBound();
               // osg::Vec3d center = boundingSphere.center();
             //   double length = boundingSphere.radius();
              //  root->addChild(drawaxis(center,length));
//                osgUtil::Optimizer optimizer;
//                optimizer.optimize(root.get());

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
                SqlHelper->closeSQLiteDB();

                //set pro
                SetProDialog setpro;
                setpro.setItem(zone_names);
                if(setpro.exec() == QDialog::Accepted)
                {

                }else
                {

                }

            }else{
                QMessageBox::critical(NULL, "critical", "can not open", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            }

            delete cptr;
            cptr=NULL;

            // _glWidget->setSceneData(root.get());
         //    _glWidget->AddModelNode(root.get());

    }else{
      //  QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));

    }


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

   // geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON );
   // geode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN );
    return geode.release();
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parent = item->parent();

    if(parent!=NULL)
    {

    int row = parent->indexOfChild(item);   
    Dialog_test dlg3;
    Initial_Aco init_a;
    Initial_Str init_s;
    SourceDlg sourcedlg;
    AddBoundAco addboundingaco;
    AddBoudingStr addboundingstr;
    LoadDlg loaddlg;
    GeneralAco generalaco;
    GeneralStr generalstr;
    Output output;

        if(parent->text(0)=="Aconstic"){

                switch(row){
                case 3:
                    if(init_a.exec()==QDialog::Accepted)
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                 break;

                case 2:
                {
                    QTreeWidgetItem *item_ = load_tree->currentItem();
                    int childcount = 0;
                    if(item_ == NULL){
                        QMessageBox::information(NULL, tr("Path"), tr("You didn't select any nodes."));
                        break;
                    }else{
                      childcount = item_->childCount();
                      QList<QString> boundingname;
                      qDebug()<<childcount;
                      if(childcount==0){
                          boundingname.push_back(item_->text(0));
                      }else
                      {
                          for(int i=0;i<childcount;i++){
                              boundingname.push_back(item_->child(i)->text(0));
                          }
                      }
                      addboundingaco.setItem(boundingname);

                    }
                    if(addboundingaco.exec()==QDialog::Accepted){


                           item->setIcon(0,QIcon(":/new/images/on.png"));
//                           qDebug()<<addboundingaco.str;
//                           QTreeWidgetItem *qwt = new QTreeWidgetItem((QStringList)addboundingaco.str);
//                           item->addChild(qwt);
//                           delete qwt;

                           item->addChild(new QTreeWidgetItem((QStringList)addboundingaco.str));
                    }
                    break;

//                case 1:
//                    dlg3.exec();
//                    item->setIcon(0,QIcon(":/new/images/on.png"));
//                    break;
                }
                case 0:
                    if(sourcedlg.exec()==QDialog::Accepted)
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                    break;

                case 4:
                    if(generalaco.exec()==QDialog::Accepted)
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                    break;

                default:
                    break;

                }


        }

        if(parent->text(0)=="structure"){

            switch(row){
                case 2:
                    if(init_s.exec()==QDialog::Accepted)
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                    break;

                case 3:
                    if(loaddlg.exec()==QDialog::Accepted)
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                    break;

                case 1:

                    if(addboundingstr.exec()==QDialog::Accepted){
                        QTreeWidgetItem *item_ = load_tree->currentItem();
                        int childcount = 0;
                        if(item_ == NULL){
                            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any nodes."));
                        }else{
                          childcount = item_->childCount();
                          QList<QString> boundingname;
                          qDebug()<<childcount;
                          if(childcount>0){
                              boundingname.push_back(item_->text(0));
                          }else
                          {
                              for(int i=0;i<childcount;i++){
                                  boundingname.push_back(item_->child(i)->text(0));
                              }
                          }

                        }
                           item->setIcon(0,QIcon(":/new/images/on.png"));
//                           qDebug()<<addboundingaco.str;
//                           QTreeWidgetItem *qwt = new QTreeWidgetItem((QStringList)addboundingaco.str);
//                           item->addChild(qwt);
//                           delete qwt;

                           item->addChild(new QTreeWidgetItem((QStringList)addboundingaco.str));
                    }
                    break;
                case 4:
                    if(generalstr.exec()==QDialog::Accepted)
                        item->setIcon(0,QIcon(":/new/images/on.png"));
                    break;

                default:
                    break;

            }

        }


        if(parent->text(0)=="ASI"){
            if(row==2){
                output.exec();
            }
        }


    }

}



void MainWindow::on_action_2_triggered()
{
    Dialog dlg2;
    if(dlg2.exec()==QDialog::Accepted){


        ui->treeWidget->setVisible(true);

        ui->label->setVisible(true);

    }
    else{
        return;
    }
}

void MainWindow::on_action_4_triggered()
{
    close();
}

void MainWindow::del(){
    delete ui->treeWidget->currentItem();
}


void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{

    if (ui->treeWidget->itemAt(pos)->parent()!=NULL&&ui->treeWidget->itemAt(pos)->parent()->text(0)=="bounding") {
          QMenu *popmenu = new QMenu( );

          QAction *action = new QAction("删除",this);
          connect(action,SIGNAL(triggered()),this,SLOT(del()));
          popmenu->addAction(action);

//          //获得全局坐标。
          auto globalPos = ui->treeWidget->mapToGlobal(pos);

          popmenu->exec(globalPos);
         delete popmenu;
    }
}

void MainWindow::on_loadtree_customContextMenuRequested(const QPoint &pos)
{
    if(load_tree->itemAt(pos)!=NULL){
    if(load_tree->itemAt(pos)->parent()==NULL){
        QMenu *pop_del_menu = new QMenu( );

        QAction *del_node_action = new QAction("删除节点",this);
        connect(del_node_action,SIGNAL(triggered()),this,SLOT(del_node()));
        pop_del_menu->addAction(del_node_action);
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


void MainWindow::del_node()
{
    QTreeWidgetItem *item_ = load_tree->currentItem();
    TreeViewItem *item = dynamic_cast<TreeViewItem*>(item_);

    if (item)
    {
        osg::ref_ptr<osg::Node> node = item->getOsgNode();
        _glWidget->ClearModelNodeByName(node);
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
        int nbases, celldim, phydim;
        char basename[33];
        ZONE *z;
        BOCO *bocos;
        int nz, nb;

        /* Checks for existance of file */
        if (!file_exists(file))
            FATAL(NULL, "File does not exist");

        /* Read CGNS file */
        printf("Reading CGNS file from %s\n", file);
        fflush(stdout);

        /* Open CGNS File */
        nbases = open_cgns(file, 0);
        if (!nbases)
            FATAL(NULL, "No bases in CGNS file");


        printf("No of Bases in file = %d\n", nbases);

        if (nbases == 1)
            cgnsbase = 1;
        else {
            printf("Give base No to browse : ");
            scanf("%d", &cgnsbase);

            if (cgnsbase < 1 || cgnsbase > nbases)
                FATAL(NULL, "Invailed base index");
        }

//        if (cg_base_read(cgnsfn, cgnsbase, basename, &celldim, &phydim))
//            FATAL(NULL, NULL);

//        printf("Using base %d - %s\n", cgnsbase, basename);
//        printf("Cell dimension     = %d\n", celldim);
//        printf("Physical dimension = %d\n", phydim);

        read_cgns();

        printf("No of zones nodes = %d\n", nZones);

        for (z = Zones, nz = 1; nz <= nZones; nz++, z++) {

            printf("\nZone No = %d\nZone Name = %s\n %d", z->id, z->name,z->nverts);
            qDebug()<<z->dim[0]*z->dim[1]*z->dim[2];
            print_ZoneType(z->type);
            switch (z->type) {
                case 2:
                    printf("Dimensions = %d x %d x %d\n",
                            z->dim[0], z->dim[1], z->dim[2]);
                    break;
                case 3:
                    printf("Dimensions = %d %d\n", z->dim[0],z->dim[1]);
                    break;
            }

for(int i=0;i<z->dim[1];i++)
{
  printf("%d %f %f %f %f\n",i,z->verts[i].x,z->verts[i].y,z->verts[i].z,z->verts[i].w);
}

            if (z->nsols) {
                printf("No of solutions node = %d\n", z->nsols);
                print_solution(z);
                SOLUTION *sols;

               // read_zone_solution(zone->id);
                sols = z->sols;
                for (int ns = 0; ns < z->nsols; ns++) {

                    printf("Size = %d\n", sols[ns].size);
                    printf("No of Fields = %d\n", sols[ns].nflds);
                    for (int nf = 0; nf < sols[ns].nflds; nf++){
                        printf("\t%s\n", sols[ns].flds[nf].name);
//                        for(int i=0;i<z->dim[0];i++)
//                        {
//                            printf("%d %f",i,sols[ns].flds[nf].data[i]) ;
//                        }
                       // printf("%d ",sols[ns].flds[nf].data[nf]) ;
                    }
                }
            }
        }

       FATAL(NULL, NULL);

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
