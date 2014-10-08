#ifndef PICKDRAGHANDLER_H
#define PICKDRAGHANDLER_H
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
class PickDragHandler:
    public osgGA::GUIEventHandler
{
public:
    PickDragHandler();


    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:

    enum Modes
    {
        VIEW =0,
        PICK =1
    };

    unsigned int mode;

    float _mx;
    float _my;

      bool addDrag(osgViewer::View *viewer, osgManipulator::Selection* selection);
    osg::ref_ptr<osgManipulator::Dragger> _activeDragger;
    osgManipulator::PointerInfo _pointer;
    bool selected;//指示是否有加过拖拽器


    osg::ref_ptr<osgManipulator::Selection> selection;
    osg::ref_ptr<osgManipulator::TranslateAxisDragger> dragger;
    osg::ref_ptr<osgManipulator::CommandManager> manager;
};


#endif // PICKDRAGHANDLER_H
