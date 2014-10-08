#include "PickHandler.h"
#include <osgViewer/View>
#include <QDebug>
#include <osgUtil/LineSegmentIntersector>
#include <osgFX/Scribe>

PickHandler::PickHandler()
{
//    manager = new osgManipulator::CommandManager;
//    selection = new osgManipulator::Selection;
//    dragger = new osgManipulator::TranslateAxisDragger;
//    d_node  = new osg::Node;
//    d_parent = new osg::MatrixTransform;
    m_dbMouseLastGetX = 0;
    m_dbMouseLastGetXNormalized = 0;
    m_dbMouseLastGetY = 0;
    m_dbMouseLastGetYNormalized = 0;

}


bool PickHandler::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
 {
       osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);

       if (!viewer) return false;
       m_pScene = dynamic_cast<osg::Group*>(viewer->getSceneData());
       switch(ea.getEventType())
       {
           case(osgGA::GUIEventAdapter::KEYUP):
           {
               if (ea.getKey()=='s')
               {
                  // saveSelectedModel(viewer->getSceneData());
               }
               else if (ea.getKey()=='o')
               {
                   osg::notify(osg::NOTICE)<<"Saved model to file 'saved_model.osg'"<<std::endl;
                  // osgDB::writeNodeFile(*(viewer->getSceneData()), "saved_model.osg");
               }
               else if (ea.getKey()=='p')
               {

               }
               else if (ea.getKey()=='c')
               {

               }
               else if (ea.getKey()==osgGA::GUIEventAdapter::KEY_Delete || ea.getKey()==osgGA::GUIEventAdapter::KEY_BackSpace)
               {

               }
               return false;
           }

           case osgGA::GUIEventAdapter::DOUBLECLICK:
           {
//               if(ea.getButton() == 1){
//                   osgUtil::PolytopeIntersector* picker= new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x-5, y-5, x+5, y+5 );
//                   osgUtil::IntersectionVisitor iv2(picker);
//                   viewer->getCamera()->accept(iv2);

//                   if(picker->containsIntersections())   {

//                   }
//               }

           }
           case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
           {

               osg::Node* scene = viewer->getSceneData();
               if (!scene) return false;

//               osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
//               osgUtil::IntersectionVisitor iv(intersector.get());
//               viewer->getCamera()->accept(iv);


//               if (intersector->containsIntersections()) {
//                   osgUtil::LineSegmentIntersector::Intersection intersection = intersector->getFirstIntersection();
//                   osg::NodePath& nodePath = intersection.nodePath;
//                qDebug()<<"get picked";
//                osg::Node* node = 0;
//                osg::Group* parent = 0;
//                node = (nodePath.size()>=1)?nodePath[nodePath.size()-1]:0;
//                parent = (nodePath.size()>=2)?dynamic_cast<osg::Group*>(nodePath[nodePath.size()-2]):0;
//                            if(!parent||!node) return false;

//                osgFX::Scribe* parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent);
//                 if(!parentAsScribe){
//                    //node not already picked,so highlight it
//                    osgFX::Scribe* scribe = new osgFX::Scribe();

//                                scribe->setWireframeColor(osg::Vec4(1.f,0.f,0.f,1.0f));
//                                scribe->addChild(node);
//                                parent->replaceChild(node,scribe);
//                     }else
//                     {
//                                //node already picked so remove
//                                osg::Node::ParentList parentList = parentAsScribe->getParents();
//                                for(osg::Node::ParentList::iterator itr=parentList.begin();
//                                    itr!=parentList.end();++itr)
//                                {
//                                    (*itr)->replaceChild(parentAsScribe,node);
//                                }
//                      }
//                }
                  double x=ea.getX();
                  double y=ea.getY();
                  osgUtil::PolytopeIntersector* picker= new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x-5, y-5, x+5, y+5 );
                  osgUtil::IntersectionVisitor iv2(picker);
                  viewer->getCamera()->accept(iv2);

                  if(picker->containsIntersections())   {
                      m_dbMouseLastGetX = ea.getX();
                      m_dbMouseLastGetXNormalized = ea.getXnormalized();
                      m_dbMouseLastGetY = ea.getY();
                      m_dbMouseLastGetYNormalized = ea.getYnormalized();

                    //  osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
                     osg::NodePath& nodePath=picker->getFirstIntersection().nodePath;

                      osg::Node* node = 0;
                      osg::Group* parent = 0;
                      node = (nodePath.size()>=1)?nodePath[nodePath.size()-1]:0;
                      parent = (nodePath.size()>=2)?dynamic_cast<osg::Group*>(nodePath[nodePath.size()-2]):0;
                      m_pPickedObject =  (nodePath.size()>=2)?dynamic_cast<osg::MatrixTransform*>(nodePath[nodePath.size()-2]):0;
                      m_mtrixOriginal = m_pPickedObject->getMatrix();
                      if(!parent||!node) return false;

                      osgFX::Scribe* parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent);
                      if(!parentAsScribe){
                       //node not already picked,so highlight it

                                  osgFX::Scribe* scribe = new osgFX::Scribe();

                                   scribe->setWireframeColor(osg::Vec4(1.f,0.f,0.f,1.0f));
                                   scribe->setWireframeLineWidth(2.0f);
                                   scribe->addChild(node);
                                   parent->replaceChild(node,scribe);
                        }else
                        {
                                   //node already picked so remove
                                   osg::Node::ParentList parentList = parentAsScribe->getParents();
                                   for(osg::Node::ParentList::iterator itr=parentList.begin();
                                       itr!=parentList.end();++itr)
                                   {
                                       (*itr)->replaceChild(parentAsScribe,node);
                                   }
                         }
                  }else{
                      m_pPickedObject = NULL;
                      return false;
                      break;
                  }


               return false;
               break;
           }
       case(osgGA::GUIEventAdapter::DRAG):
       {
         if(m_pPickedObject == NULL){
             return false;
             break;
         }
         double dPositionX = ea.getXnormalized() - m_dbMouseLastGetXNormalized;
         double dPositionY = ea.getYnormalized() - m_dbMouseLastGetYNormalized;
         if(fabs(dPositionX) < 0.01 && fabs(dPositionY) < 0.01)
             return false;
         osg::Matrixd mat = viewer->getCameraManipulator()->getMatrix();
         osg::Vec3d lookVector(mat(2,0),mat(2,1),mat(2,2));
         osg::Vec3d eyeVector(mat(3,0),mat(3,1),mat(3,2));
         double moveFactor = 0.3 * (lookVector - eyeVector).length();
         osg::Matrix mtrx;
         mtrx = m_mtrixOriginal * osg::Matrix::translate(
                     moveFactor*(dPositionX), 0.0, moveFactor*(dPositionY));
         m_pPickedObject->setMatrix(mtrx);
         viewer->setSceneData(m_pScene);
         return false;
         break;

       }
           default:
               return false;
       }
   }
