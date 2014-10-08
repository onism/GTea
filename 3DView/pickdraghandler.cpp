#include "pickdraghandler.h"
#include <osgViewer/View>
#include <QDebug>
#include <osgUtil/LineSegmentIntersector>
#include <osgFX/Scribe>

PickDragHandler::PickDragHandler():_activeDragger(0),
    _mx(0.0f),
    _my(0.0f),
    mode(VIEW)
{
    manager = new osgManipulator::CommandManager;
    selection = new osgManipulator::Selection;
    dragger = new osgManipulator::TranslateAxisDragger;
}


bool  PickDragHandler::addDrag(osgViewer::View* viewer, osgManipulator::Selection* selection)
{
       osg::ref_ptr<osg::Group> group = viewer->getSceneData()->asGroup();
      if(!selected) //如果没有加拖拽器，则加上,其中selection相当于一个节点node来处理
      {
            float scale = selection->getBound().radius() * 1.6;
            dragger->setupDefaultGeometry();
            dragger->setMatrix( osg::Matrix::scale(scale, scale, scale) *
            osg::Matrix::translate(selection->getBound().center()) );//设置拖拽器的大小

            group->addChild(dragger.get());//将拖拽器加到场景中
            //viewer->setSceneData(group);

            manager->connect( *dragger, *selection );//连接拖拽器和selection
            selected = true;
        }
        else //如果已加拖拽器，则去掉
        {
             group->removeChild(dragger.get());
             manager->disconnect(*dragger);
             viewer->setSceneData(group);
             selected = false;

         }
        return true;
}

bool PickDragHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
    osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
              if (!view) return false;

              if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L&&
                  ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN &&
                  _activeDragger == 0)
              {
                  mode = ! mode;

              }

              if (VIEW == mode){
                  //view


                  switch(ea.getEventType())
                  {


                      case osgGA::GUIEventAdapter::DOUBLECLICK:
                      {

                      }
                      case osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON:
                      {

                          osg::Node* scene = view->getSceneData();
                          if (!scene) return false;

                             double x=ea.getX();
                             double y=ea.getY();
                             osgUtil::PolytopeIntersector* picker= new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x-5, y-5, x+5, y+5 );
                             osgUtil::IntersectionVisitor iv2(picker);
                             view->getCamera()->accept(iv2);

                             if(picker->containsIntersections())   {

                               //  osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
                                osg::NodePath& nodePath=picker->getFirstIntersection().nodePath;

                                 osg::Node* node = 0;
                                 osg::Group* parent = 0;
                                 node = (nodePath.size()>=1)?nodePath[nodePath.size()-1]:0;
                                 parent = (nodePath.size()>=2)?dynamic_cast<osg::Group*>(nodePath[nodePath.size()-2]):0;

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

                                 return false;
                                 break;
                             }


                          return false;
                          break;
                      }

                      default:
                          return false;
                  }

              }  else{
                  //drag
                  switch (ea.getEventType())
                  {
                      case osgGA::GUIEventAdapter::PUSH:
                      {
                          osgUtil::LineSegmentIntersector::Intersections intersections;

                          _pointer.reset();

                          if (view->computeIntersections(ea.getX(),ea.getY(),intersections))
                          {

                              _pointer.setCamera(view->getCamera());
                              _pointer.setMousePosition(ea.getX(), ea.getY());

                              for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
                                  hitr != intersections.end();
                                  ++hitr)
                              {
                                  _pointer.addIntersection(hitr->nodePath, hitr->getLocalIntersectPoint());
                              }
                              for (osg::NodePath::iterator itr = _pointer._hitList.front().first.begin();
                                   itr != _pointer._hitList.front().first.end();
                                   ++itr)
                              {
                                  osgManipulator::Dragger* dragger = dynamic_cast<osgManipulator::Dragger*>(*itr);
                                  if (dragger)
                                  {

                                      dragger->handle(_pointer, ea, aa);
                                      _activeDragger = dragger;
                                      break;
                                  }
                              }
                          }
                      }
                      case osgGA::GUIEventAdapter::DRAG:
                      case osgGA::GUIEventAdapter::RELEASE:
                      {
                          if (_activeDragger)
                          {
                              _pointer._hitIter = _pointer._hitList.begin();
                              _pointer.setCamera(view->getCamera());
                              _pointer.setMousePosition(ea.getX(), ea.getY());

                              _activeDragger->handle(_pointer, ea, aa);
                          }

                          if ( ea.getEventType()==osgGA::GUIEventAdapter::RELEASE )
                           {
                                  _activeDragger = NULL;
                                  _pointer.reset();
                            }
                          break;
                      }
                      //select object to drag
                     case osgGA::GUIEventAdapter::DOUBLECLICK:
                     {
                      if(!selected)//如果没有加任何拖拽器，则对选中的加上一个
                     {
                          osgUtil::LineSegmentIntersector::Intersections inters;
                          if(view->computeIntersections(ea.getX(), ea.getY(), inters))//鼠标点击点发一条射线，看是否viewer中有物体的交线
                          {
                               //将第一个碰撞到的物体保存到迭代器
                              osgUtil::LineSegmentIntersector::Intersections::iterator hitr=inters.begin();

                               osg::NodePath nodePath = hitr->nodePath;

                               //遍历第一个物体的所有nodePath，找出selection的名字，从而找到点击的模型
                              for(osg::NodePath::iterator iterNode = nodePath.begin(); iterNode != nodePath.end(); iterNode++)
                              {
                                    if((*iterNode)->getName() == "selectionroot")//nodePath中包括了几个内容，其中有selection的名字
                                    {
                                           std::cout<<"撞到了："<<(*iterNode)->getName()<<std::endl;
                                           selection = dynamic_cast<osgManipulator::Selection*>(*iterNode);
                                           if(addDrag(view, selection))  //加上拖拽器
                                           {
                                                 std::cout<<"selection27加上drag"<<std::endl;
                                           }
                                     }

                              }
                            }
                         }
                         else
                         {
                            addDrag(view, selection);//如果存在一个拖拽器（私有变量selection保存了对应的node），则去掉这个拖拽器
                         }
                        break;


                     }



              default:
                  break;
                  }

                  if (ea.getEventType() == osgGA::GUIEventAdapter::RELEASE)
                  {
                      _activeDragger = 0;
                      _pointer.reset();
                  }

                  return true;
              }



}

