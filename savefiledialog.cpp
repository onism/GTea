#include "savefiledialog.h"
#include "ui_savefiledialog.h"
#include "changepositiondialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QTreeWidgetItem>
#include <QMouseEvent>
#include <QTreeWidget>
#include <QDebug>
#include "sqlhelper.h"
#include "3DView/dbcgns.h"
#include "3DView/corelib.h"
SaveFileDialog::SaveFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveFileDialog)
{
    ui->setupUi(this);
    cgnsIO = new DBCGNS;

    ui->treeWidget->setHeaderLabel("文件");
}

SaveFileDialog::~SaveFileDialog()
{
    filetree.clear();
    if (cgnsIO != NULL) {
                         // cgnsIO->Reset();
                        delete cgnsIO;
                        cgnsIO = NULL;
                    }
    delete ui;
}


void SaveFileDialog::setchangedfile(QMap<int, ChangeObject> objects)
{
    filetree.clear();

    QMap<int,ChangeObject>::const_iterator map_i;
    for(map_i=objects.begin();map_i!= objects.end();++map_i)
    {
       if(map_i.key() < 0){ // file is already changed
           QString filename = map_i.value().filename;
           QString zonename = map_i.value().zonename;

           QString trans_change = QString("平移变化：x: '%f' y: 'f', z:'%f'").arg(map_i.value().trans_x).arg(map_i.value().trans_y).arg(map_i.value().trans_z);
           QString scale_change = QString("缩放变化：x: '%f' y: 'f', z:'%f'").arg(map_i.value().x_scale).arg(map_i.value().y_scale).arg(map_i.value().z_scale);
           QString angle_change = QString("缩放变化：x: '%f' y: 'f', z:'%f'").arg(map_i.value().angle_x).arg(map_i.value().angle_y).arg(map_i.value().angle_z);

           // QString trans_change="平移变化：x:"+ map_i.value().trans_x+" y:"+map_i.value().trans_y+" z:"+map_i.value().trans_z+"\r\n";
         //  QString scale_change="缩放变化：x"+ map_i.value().x_scale+" y:"+map_i.value().y_scale+" z:"+map_i.value().z_scale+"\r\n";
         //  QString angle_change="角度变化：x" + map_i.value().angle_x+" y:"+map_i.value().angle_y+" z"+map_i.value().angle_z+"\r\n";
           QString info = trans_change + scale_change + angle_change;
           if(filetree.contains(filename))
           {
               QMap<QString,ChangeObject> value =filetree.value(filename);
               value.insert(zonename,map_i.value());
               filetree.insert(filename,value);
           }else{
               QMap<QString,ChangeObject> value;
               value.insert(zonename,map_i.value());
               filetree.insert(filename,value);
           }
//           filechangeinfo<<filename<<zonename;
//           QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget, filechangeinfo);
//           ui->treeWidget->addTopLevelItem(root);

       }
    }
    construct_tree();
}

void SaveFileDialog::construct_tree()
{
    if(!filetree.isEmpty())
    {
        foreach (QString name, filetree.keys())
        {
            QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget,QStringList(name) );
            root->setCheckState(0,Qt::Checked);
            ui->treeWidget->addTopLevelItem(root);
            QMap<QString,ChangeObject> value = filetree[name];

            foreach (QString zonename, value.keys())
            {
                QTreeWidgetItem *child = new QTreeWidgetItem(root, QStringList(zonename));
                child->setCheckState(0,Qt::Checked);
                root->addChild(child);
            }



        }

    }
}

void SaveFileDialog::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* item = ui->treeWidget->itemAt(pos);
    if (item->parent()==NULL) {

      showContextMenu(item,ui->treeWidget->mapToGlobal(pos));
   }

}

void SaveFileDialog::showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos) {
     QMenu menu;
     QAction *save_action = new QAction("save file",this);
     connect(save_action,SIGNAL(triggered()),this,SLOT(save_file()));
     menu.addAction(save_action);
     menu.exec(globalPos);
 }
void SaveFileDialog::save_file()
{
     QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save  file"), "",
             tr("cgns (*.cgns);;All Files (*)"));



    if (fileName.isEmpty())
            return;
     else {
//        QFile file(fileName);
//       if (!file.open(QIODevice::WriteOnly)) {
//                QMessageBox::information(this, tr("Unable to open file"),
//                    file.errorString());
//                return;
//       }
       //get filepath where the orign file
       QString origin_filename = ui->treeWidget->currentItem()->text(0);
       QString sql = "select distinct filepath from cgns where filename ='";
       sql +=origin_filename;
       sql +="'";
       qDebug()<<sql;
       QList<QString>filepathlist = SQLHelper::selectbysql(sql);
       if(!filepathlist.isEmpty()){
           QString filepath = filepathlist.at(0);

         if(file_exists(filepath.toStdString().c_str())){
           //read origin cgns
             qDebug()<<filepath;

          cgnsIO->Set_InputGrid_Filename(filepath.toStdString().c_str()) ;
          //cgnsIO->Set_InputParam_Filename(filepath.toStdString().c_str());
          //cgnsIO->Set_InputSolution_Filename(filepath.toStdString().c_str());
          cgnsIO->Read_DB();
          ROOT* CoreDB = cgnsIO->Get_DB();
          qDebug()<<"read cgns end";
          QTreeWidgetItem* item = ui->treeWidget->currentItem();
          int childcount = item->childCount();
          //scan which zone should save

          for(int i=0;i<childcount;i++)
          {
            if(item->child(i)->checkState(0)== Qt::Checked){
                //scan the cgns file
                QString select_zonename = item->child(i)->text(0);
                qDebug()<<select_zonename;
               for(int j=0;j<CoreDB->bases[0].nzones;j++){
                   qDebug()<<CoreDB->bases[0].zones[j].name<<select_zonename.toStdString().c_str();
                   //if(select_zonename.toStdString().c_str() == CoreDB->bases[0].zones[j].name){
                    if(!select_zonename.toStdString().compare(CoreDB->bases[0].zones[j].name)){
                       //change the coord
                       //find the change value from map
                        qDebug()<<CoreDB->bases[0].zones[j].name;
                       foreach (QString name, filetree.keys())
                       {
                           qDebug()<<name<<origin_filename;
                          if(name == origin_filename){
                           QMap<QString,ChangeObject> value = filetree[name];
                           foreach (QString zonename, value.keys())
                           {
                               qDebug()<<zonename<<select_zonename;
                              if(zonename == select_zonename )
                              {
                                 ChangeObject object = value.value(zonename);
                                 //change coord
                                for(int k=0;k< CoreDB->bases[0].zones[j].nverts;k++) {
                                    //trans should +
                                    CoreDB->bases[0].zones[j].verts[k].x += object.trans_x;
                                    CoreDB->bases[0].zones[j].verts[k].y += object.trans_y;
                                    CoreDB->bases[0].zones[j].verts[k].z += object.trans_z;
                                    //scale should * and need check >0
                                    if(object.x_scale > 0){
                                         CoreDB->bases[0].zones[j].verts[k].x *= object.x_scale;
                                    }
                                    if(object.y_scale > 0)
                                    {
                                         CoreDB->bases[0].zones[j].verts[k].y *= object.y_scale;
                                    }
                                    if(object.z_scale>0){
                                        CoreDB->bases[0].zones[j].verts[k].z *= object.z_scale;
                                    }
                                    //FIXED:ME angel should think?????

                                }
                                 break;
                              }
                           }// map zone name end
                           break;
                          }
                       }//map file name end
                   }
               }//cgns zonename end
            }
          }//tree child end
          //save file
          qDebug()<<"write file begin";

           cgnsIO->Set_OutputGrid_Filename(fileName.toStdString().c_str());
          // cgnsIO->Set_OutputSolution_Filename(fileName.toStdString().c_str());
          // cgnsIO->Set_OutputParam_Filename(fileName.toStdString().c_str());
          // Set to Overwrite Mode
           cgnsIO->Set_OutputGrid_IOMode(3);
         //  cgnsIO->Set_OutputSolution_IOMode(3);
         //  cgnsIO->Set_OutputParam_IOMode(3);

           cgnsIO->Write_DB();
       //  cgnsIO->Reset();
  qDebug()<<"write file end";
//  CoreDB = NULL;
//  item = NULL;



         }// origin file is exist;
       }else{
           QMessageBox::information(this, tr("Unable to open file"),"select a file");
       }

    }//save file is not empty
}

void SaveFileDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save  file"), "",
            tr("cgns (*.cgns);;All Files (*)"));



   if (fileName.isEmpty())
           return;
    else {
//        QFile file(fileName);
//       if (!file.open(QIODevice::WriteOnly)) {
//                QMessageBox::information(this, tr("Unable to open file"),
//                    file.errorString());
//                return;
//       }
      //get filepath where the orign file
      QString origin_filename = ui->treeWidget->currentItem()->text(0);
      QString sql = "select distinct filepath from cgns where filename ='";
      sql +=origin_filename;
      sql +="'";
      qDebug()<<sql;
      QList<QString>filepathlist = SQLHelper::selectbysql(sql);
      if(!filepathlist.isEmpty()){
          QString filepath = filepathlist.at(0);

        if(file_exists(filepath.toStdString().c_str())){
          //read origin cgns
            qDebug()<<filepath;

         cgnsIO->Set_InputGrid_Filename(filepath.toStdString().c_str()) ;
         //cgnsIO->Set_InputParam_Filename(filepath.toStdString().c_str());
         //cgnsIO->Set_InputSolution_Filename(filepath.toStdString().c_str());
         cgnsIO->Read_DB();
         ROOT* CoreDB = cgnsIO->Get_DB();

         //scan which zone should save
               QString select_zonename = "fluid";
               qDebug()<<select_zonename;
              for(int j=0;j<CoreDB->bases[0].nzones;j++){
                  qDebug()<<CoreDB->bases[0].zones[j].name<<select_zonename.toStdString().c_str();
                  //if(select_zonename.toStdString().c_str() == CoreDB->bases[0].zones[j].name){
                   if(!select_zonename.toStdString().compare(CoreDB->bases[0].zones[j].name)){
                      //change the coord
                      //find the change value from map
                       qDebug()<<CoreDB->bases[0].zones[j].name;
                      foreach (QString name, filetree.keys())
                      {
                          qDebug()<<name<<origin_filename;
                         if(name == origin_filename){
                          QMap<QString,ChangeObject> value = filetree[name];
                          foreach (QString zonename, value.keys())
                          {
                              qDebug()<<zonename<<select_zonename;
                             if(zonename == select_zonename )
                             {
                                ChangeObject object = value.value(zonename);
                                //change coord
                               for(int k=0;k< CoreDB->bases[0].zones[j].nverts;k++) {
                                   //trans should +
                                   CoreDB->bases[0].zones[j].verts[k].x += object.trans_x;
                                   CoreDB->bases[0].zones[j].verts[k].y += object.trans_y;
                                   CoreDB->bases[0].zones[j].verts[k].z += object.trans_z;
                                   //scale should * and need check >0
                                   if(object.x_scale > 0){
                                        CoreDB->bases[0].zones[j].verts[k].x *= object.x_scale;
                                   }
                                   if(object.y_scale > 0)
                                   {
                                        CoreDB->bases[0].zones[j].verts[k].y *= object.y_scale;
                                   }
                                   if(object.z_scale>0){
                                       CoreDB->bases[0].zones[j].verts[k].z *= object.z_scale;
                                   }
                                   //FIXED:ME angel should think?????

                               }
                                break;
                             }
                          }// map zone name end
                          break;
                         }
                      }//map file name end
                  }
              }//cgns zonename end
           }
         }//tree child end
         //save file
         qDebug()<<"write file begin";

          cgnsIO->Set_OutputGrid_Filename(fileName.toStdString().c_str());
         // cgnsIO->Set_OutputSolution_Filename(fileName.toStdString().c_str());
         // cgnsIO->Set_OutputParam_Filename(fileName.toStdString().c_str());
         // Set to Overwrite Mode
          cgnsIO->Set_OutputGrid_IOMode(3);
        //  cgnsIO->Set_OutputSolution_IOMode(3);
        //  cgnsIO->Set_OutputParam_IOMode(3);

          cgnsIO->Write_DB();
      //  cgnsIO->Reset();
 qDebug()<<"write file end";
//  CoreDB = NULL;
//  item = NULL;



        }// origin file is exist;


}
