#ifndef NEWDRAGGER_H
#define NEWDRAGGER_H

#include <osgManipulator/Translate1DDragger>
class NewDragger : public osgManipulator::CompositeDragger
{
public:
    NewDragger();
    void setupDefaultGeometry();
protected:
    ~NewDragger();

    osg::ref_ptr<osgManipulator::Translate1DDragger> _xDragger;
    osg::ref_ptr<osgManipulator::Translate1DDragger> _yDragger;
    osg::ref_ptr<osgManipulator::Translate1DDragger> _zDragger;
};

#endif // NEWDRAGGER_H
