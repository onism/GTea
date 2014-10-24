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
#include <osgText/Text>
class PickDragHandler:
    public osgGA::GUIEventHandler
{
public:
    PickDragHandler(osgText::Text* updateText);


    bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
    void refreshHUD(osgViewer::View *viewer, const osgGA::GUIEventAdapter& ea);
    osg::ref_ptr<osgText::Text> _updateText;
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

    void setLabel(const std::string& name){
            if(_updateText.get()) _updateText->setText(name);
        }
    osg::ref_ptr<osgManipulator::Selection> selection;
    osg::ref_ptr<osgManipulator::TranslateAxisDragger> dragger;
    osg::ref_ptr<osgManipulator::CommandManager> manager;
    osg::Vec3 position;
    osg::Vec3 center;
    osg::Vec3 up;
};


#endif // PICKDRAGHANDLER_H
