#ifndef PICKHANDLER_H
#define PICKHANDLER_H
#include <osgUtil/LineSegmentIntersector>
#include <osgGA/GUIEventHandler>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>
#include <osgViewer/View>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TrackballDragger>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgManipulator/Selection>
#include <osgManipulator/CommandManager>
#include <osgManipulator/TranslateAxisDragger>
class PickHandler: public osgGA::GUIEventHandler
{
public:
    PickHandler( );

    ~PickHandler() {}

     virtual   bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );
protected:
    float _mx,_my;
//    osg::ref_ptr<osgManipulator::Dragger> dragger_pick;
//    osg::ref_ptr<osgManipulator::Dragger> _activeDragger;
//    osgManipulator::PointerInfo _pointer;

//    osg::ref_ptr<osgManipulator::CommandManager> manager;
//    osg::ref_ptr<osgManipulator::Selection> selection;
//    std::list<osg::ref_ptr<osg::MatrixTransform>> selections;
//    osg::ref_ptr<osgManipulator::TranslateAxisDragger> dragger;
//    osg::ref_ptr<osg::Node> d_node;
//    osg::ref_ptr<osg::MatrixTransform> d_parent;
//    osg::ref_ptr<osg::Group> d_root;

private:
    osg::ref_ptr<osg::Group> m_pScene;
    osg::ref_ptr<osg::MatrixTransform> m_pPickedObject;
    osg::Matrixd m_mtrixOriginal;
    osg::Matrixd m_mtrxMatrix;

    int m_nTransformSelection;
    double m_dbMouseLastGetXNormalized;
    double m_dbMouseLastGetYNormalized;
    double m_dbMouseLastGetX;
    double m_dbMouseLastGetY;


};

#endif // PICKHANDLER_H
