#include "loadmsh.h"
#include <QFile>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QList>
#include <QtCore>
#include <QtGui/QtGui>
#include <QMessageBox>

#include <osgQt/GraphicsWindowQt>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osgViewer/ViewerEventHandlers>
#include <osg/LineWidth>
#include <osgUtil/SmoothingVisitor>
#include <osg/PolygonMode>
#include <osg/BlendFunc>
LoadMsh::LoadMsh(QString filename):oroot(NULL)

{
   if(openfile(filename)){
       drawmsh();
   }else{
      QMessageBox::critical(NULL, "critical", "open file error", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
   }

}


int LoadMsh::openfile(QString filename)
{
     QFile mshfile(filename);
    if ( mshfile.open( QIODevice::ReadOnly|QIODevice::Text ) ) {
            printf("read msh");
             QTextStream stream( &mshfile );
             QString line;
             int index=0;
                 int vertices_number=0 ;
                 int face_number = 0;

                  QList<MshNodeData> data;
                 int zone_id=0,start_vertice=0,end_vertice=0,type=0,dim=0;

                 int face_zone_id=0,start_face=0,end_face=0,face_type=0,elem_type=0;

             while ( !stream.atEnd() ) {
                 line = stream.readLine(); // 不包括“/n”的一行文本
                 char*  ch;
                 QByteArray ba = line.toLatin1();
                 ch=ba.data();
                 if(line.endsWith(")(")){
                     qDebug()<<line;
                     sscanf(ch,"(%d",&index);
                     switch (index) {
                     case 10:
                     {
                         qDebug()<<"read nodes";
                         MshNode msh_node;
                         int n= sscanf(ch,"(%d (%d %x %x %d %d)(",&index,&zone_id,&start_vertice,&end_vertice,&type,&dim);
                         if(zone_id == 0)
                         {
                             QMessageBox::critical(NULL, "critical", "no node data", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                             return 0;
                         }
                         if( n != 6){
                             n= sscanf(ch,"(%d (%d %x %x %d %d)(",&index,&zone_id,&start_vertice,&end_vertice,&type);
                             if(n!=5){
                                 QMessageBox::critical(NULL, "critical", "node header error", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                                 return 0;
                             }
                         }
                         qDebug()<<"sscanf read"<<n;
                         vertices_number = end_vertice - start_vertice + 1;
                         msh_node.first_index = start_vertice;
                         msh_node.last_index  = end_vertice;
                         msh_node.type        = type;
                         msh_node.nd          = dim;
                         msh_node.zone_id     = index;
                         msh_node.data  =read_zones(vertices_number,stream,dim);

                        // qDebug()<<data.size()<<"data size"<<vertices_number;
                         zone_nodes.push_back(msh_node);
                         qDebug()<<zone_nodes.size()<<"zone siez";
                         break;
                     }//read nodes
                     case 13:
                     {
                         qDebug()<<"read face";
                         int n = sscanf(ch,"(%d (%x %x %x %d %d)(",&index,&face_zone_id,&start_face,&end_face,&face_type,&elem_type);
                         if( n!= 6)
                         {
                             QMessageBox::critical(NULL, "critical", " Reading faces\n bad header", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                             return 0;
                         }

                         if(face_type == 2 ){ // inter
                             break;
                         }else{
                         face_number = end_face - start_face+1;
                         qDebug()<<face_number;
                         QList<FaceData> face_data = read_face(face_number,stream,elem_type);
                         qDebug()<<face_data.size()<<"---"<<face_number<<face_data.at(face_number-1).cl;
                         MshFaces faces;
                         faces.zone_id = face_zone_id;
                         faces.facedatas = face_data;

                         faces_nodes.push_back(faces);
                         qDebug()<<faces_nodes.size();
                         break;
                         }
                     }

                     default:
                         break;
                     }


                   continue;
                 }else if(line.endsWith("))")){
                      sscanf(ch,"(%d",&index);
                      if(index == 45){


                          Name_Id name_id;
                          sscanf(ch,"(%d (%d %s %[^)])())",&index,&name_id.zone_id,name_id.type,name_id.name);
                          Face_Names.push_back(name_id);
                          continue;
                      }
                 }
             }
             mshfile.close();
             return 1;
         }else
    {
        mshfile.close();
        return 0;
    }
}
QList<FaceData> LoadMsh::read_face(int data_numbers,QTextStream &stream,int elem_type)
{
        QList<FaceData> facedata;
        qDebug()<<data_numbers;
        if(elem_type == 0){ // mixed type
        for(int i=0; i < data_numbers ; i++ ){
            QString line = stream.readLine();
            char*  ch;
            QByteArray ba = line.toLatin1();
            ch=ba.data();
            int type;
            sscanf(ch,"%d",&type);
            FaceData fdata;
            fdata.type = type;

            fdata.vertics = new int[type];
            switch (type) {
            case 4:
                sscanf(ch,"%d %x %x %x %x %x %x",&type,&fdata.vertics[0],&fdata.vertics[1],&fdata.vertics[2],&fdata.vertics[3],&fdata.cl,&fdata.cr);
              //   qDebug()<<fdata.vertics[0]<<fdata.vertics[1]<<fdata.vertics[2]<<fdata.vertics[3]<<"-------"<<i;
                break;

            case 2:
                sscanf(ch,"%d %x %x  %x %x",&type,&fdata.vertics[0],&fdata.vertics[1],&fdata.cl,&fdata.cr);
              //  qDebug()<<fdata.vertics[0]<<fdata.vertics[1]<<"-------"<<i;
                break;
            case 3:
                sscanf(ch,"%d %x %x %x  %x %x",&type,&fdata.vertics[0],&fdata.vertics[1],&fdata.vertics[2],&fdata.cl,&fdata.cr);
                break;
            default:

               break;
            }
            facedata.push_back(fdata);

        }
        return facedata;
        }else if(elem_type == 3){ //read tri
            for(int i=0; i < data_numbers ; i++ ){
                QString line = stream.readLine();
                char*  ch;
                QByteArray ba = line.toLatin1();
                ch=ba.data();
                FaceData fdata;
                fdata.type = 3;
                fdata.vertics = new int[3];

                sscanf(ch,"%x %x %x %x %x",&fdata.vertics[0],&fdata.vertics[1],&fdata.vertics[2],&fdata.cl,&fdata.cr);
                  //   qDebug()<<fdata.vertics[0]<<fdata.vertics[1]<<fdata.vertics[2]<<fdata.vertics[3]<<"-------"<<i;

                facedata.push_back(fdata);

            }
            return facedata;
        }else if(elem_type == 2 ){ //read line
            for(int i=0; i < data_numbers ; i++ ){
                QString line = stream.readLine();
                char*  ch;
                QByteArray ba = line.toLatin1();
                ch=ba.data();
                FaceData fdata;
                fdata.type = 2;
                fdata.vertics = new int[2];

                sscanf(ch,"%x %x %x %x ",&fdata.vertics[0],&fdata.vertics[1],&fdata.cl,&fdata.cr);
                  //   qDebug()<<fdata.vertics[0]<<fdata.vertics[1]<<fdata.vertics[2]<<fdata.vertics[3]<<"-------"<<i;

                facedata.push_back(fdata);

            }
            return facedata;
        }else if(elem_type == 4){
            for(int i=0; i < data_numbers ; i++ ){ //read quard
                QString line = stream.readLine();
                char*  ch;
                QByteArray ba = line.toLatin1();
                ch=ba.data();
                FaceData fdata;
                fdata.type = 4;
                fdata.vertics = new int[4];

                sscanf(ch,"%x %x %x %x %x %x",&fdata.vertics[0],&fdata.vertics[1],&fdata.vertics[2],&fdata.vertics[3],&fdata.cl,&fdata.cr);
                  //   qDebug()<<fdata.vertics[0]<<fdata.vertics[1]<<fdata.vertics[2]<<fdata.vertics[3]<<"-------"<<i;

                facedata.push_back(fdata);

            }
            return facedata;
        }
}

QList<MshNodeData> LoadMsh::read_zones(int data_numbers, QTextStream &stream, int dim)
{
        QList<MshNodeData> data;
        for(int i=0; i < data_numbers; i++){
            QString  line = stream.readLine(); // 不包括“/n”的一行文本
            char*  ch;
            QByteArray ba = line.toLatin1();
            ch=ba.data();
            MshNodeData node;
            int n =   sscanf(ch,"%f %f %f",&node.x,&node.y,&node.z);
            if( n == 2){
                node.z = 0.0;
                n = 3;
            }
             data.push_back(node);
//            if(dim == 3 ){
//                MshNodeData node;
//                sscanf(ch,"%f %f %f",&node.x,&node.y,&node.z);

//                data.push_back(node);
//            }else if(dim == 2){
//                MshNodeData node;
//                node.z = 0.0;
//                sscanf(ch,"%f %f ",&node.x,&node.y);

//                data.push_back(node);
//            }
           if( n != 3 )
           {
               QMessageBox::critical(NULL, "critical", "node line error", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
               return data;
           }

        }

        return data;
}

MshNode LoadMsh::getMshNode_by_zone_id(int zone_id){
//    int zones_number = zone_nodes.size();
//    for(int i=0;i<zones_number;i++){
//        if(zone_id == zone_nodes.at(i).zone_id){
//            return zone_nodes.at(i);
//        }
//    }
//    return NULL;
return zone_nodes.at(0);
}

int LoadMsh::getFaceNameById(int zone_id, char *buf){
    int face_name_number = Face_Names.size();
    for(int i=0;i<face_name_number;i++){
        if(zone_id == Face_Names.at(i).zone_id){
            //buf = Face_Names.at(i).name;
            strcpy(buf,Face_Names.at(i).name);
            strcat(buf,"  type ");
            strcat(buf,Face_Names.at(i).type);

            if(!strcmp(Face_Names.at(i).type,"wall"))
                return 2;//is wall should visualization
            else
                return 1;//do not need
        }
    }
    return 0;
}

int LoadMsh::drawmsh()
{
    face_wall_names.clear();
             int faces_number = faces_nodes.size();
             qDebug()<<"draw"<<faces_number;
             oroot = new osg::Group;
             if(faces_number > 0){
                 //testing draw
                 for(int i=0;i<faces_number;i++){
                      osg::ref_ptr<osg::Geode> geode = new osg::Geode;
                      osg::StateSet* ss = geode->getOrCreateStateSet();
                      ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
                      MshFaces f = faces_nodes.at(i);
                      MshNode  node = getMshNode_by_zone_id(f.zone_id);
                      if(node.data.size()<=0){
                         QMessageBox::critical(NULL, "critical", "no node data", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                          return 0;
                      }
                      char buf[512];
                     if( getFaceNameById(f.zone_id,buf)!=2)
                         continue;
                      geode->setName(buf);
                      face_wall_names.push_back(QString::fromLatin1(buf));
                      for(int j = 0; j< f.facedatas.size();j++){
                 //        qDebug()<<j;
                          osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();
                          osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
                          geom->setVertexArray(v.get());
                          if(f.facedatas.at(j).type == 2){//lines
                              int  l0 = f.facedatas.at(j).vertics[0]-1;
                              int  l1 = f.facedatas.at(j).vertics[1]-1;
                               v->push_back(osg::Vec3(node.data[l0].x,node.data[l0].y,node.data[l0].z));
                               v->push_back(osg::Vec3(node.data[l1].x,node.data[l1].y,node.data[l1].z));
                                osg::LineWidth *lw= new osg::LineWidth;
                                lw->setWidth(1.0);
                                ss->setAttribute(lw);
                                osg::BlendFunc* blendFunc = new osg::BlendFunc;
                                  blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                                  ss->setAttributeAndModes(blendFunc);
                                  ss->setMode(GL_BLEND, osg::StateAttribute::ON);
                               geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
                           }else if(f.facedatas.at(j).type == 3){
                              int  t0 = f.facedatas.at(j).vertics[0]-1;
                              int  t1 = f.facedatas.at(j).vertics[1]-1;
                              int  t2 = f.facedatas.at(j).vertics[2]-1;
                              v->push_back(osg::Vec3(node.data.at(t0).x,node.data.at(t0).y,node.data.at(t0).z));

                              v->push_back(osg::Vec3(node.data.at(t1).x,node.data.at(t1).y,node.data.at(t1).z));

                              v->push_back(osg::Vec3(node.data.at(t2).x,node.data.at(t2).y,node.data.at(t2).z));

                              geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3));
                          }else if(f.facedatas.at(j).type == 4){
                              int  q0 = f.facedatas.at(j).vertics[0]-1;
                              int  q1 = f.facedatas.at(j).vertics[1]-1;
                              int  q2 = f.facedatas.at(j).vertics[2]-1;
                              int  q3 = f.facedatas.at(j).vertics[3]-1;

                              v->push_back(osg::Vec3(node.data.at(q0).x,node.data.at(q0).y,node.data.at(q0).z));
                              v->push_back(osg::Vec3(node.data.at(q1).x,node.data.at(q1).y,node.data.at(q1).z));
                              v->push_back(osg::Vec3(node.data.at(q2).x,node.data.at(q2).y,node.data.at(q2).z));
                              v->push_back(osg::Vec3(node.data.at(q3).x,node.data.at(q3).y,node.data.at(q3).z));

                              geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
                          }

                            ss->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));

                         geode->addDrawable( geom.get());
                      }
                      oroot->addChild(geode.get());

                 }
                     return 1;

}
}
