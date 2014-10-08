#include "newdragger.h"

#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <osg/Quat>
#include <osgManipulator/Dragger>
#include <osg/LineWidth>
NewDragger::NewDragger()
{
    _xDragger = new osgManipulator::Translate1DDragger(osg::Vec3(0.0,0.0,0.0),osg::Vec3(0.0,0.0,1.0));
    addChild(_xDragger.get());
    addDragger(_xDragger.get());

    _yDragger = new osgManipulator::Translate1DDragger(osg::Vec3(0.0,0.0,0.0),osg::Vec3(0.0,0.0,1.0));
    addChild(_yDragger.get());
    addDragger(_yDragger.get());

    _zDragger = new osgManipulator::Translate1DDragger(osg::Vec3(0.0,0.0,0.0),osg::Vec3(0.0,0.0,1.0));
    addChild(_zDragger.get());
    addDragger(_zDragger.get());

    setParentDragger(getParentDragger());
}

NewDragger::~NewDragger()
{

}

void NewDragger::setupDefaultGeometry()
{
    osg::ref_ptr<osg::Geode> lineGeode = new osg::Geode;
    osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
    osg::ref_ptr< osg::Vec3Array > vectic = new osg::Vec3Array;
    vectic->push_back(osg::Vec3(0.0f,0.0f,0.0f));
    vectic->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    geo->setVertexArray(vectic);
    geo->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
    lineGeode->addDrawable(geo);

    //set line width
    osg::ref_ptr<osg::LineWidth> linewidth = new osg::LineWidth;
    linewidth->setWidth(2.5f);
    lineGeode->getOrCreateStateSet()->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
    lineGeode->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

    _xDragger->addChild(lineGeode);
    _yDragger->addChild(lineGeode);
    _zDragger->addChild(lineGeode);

    //create an invisible cylinder for picking the line
    osg::ref_ptr<osg::Geode> topgeode = new osg::Geode;
    osg::ref_ptr<osg::Cone> cone = new osg::Cone(osg::Vec3(0.0f,0.0f,1.0f),0.04f,0.15f);
    topgeode->addDrawable(new osg::ShapeDrawable(cone));

    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0.0f,0.0f,0.5f),0.075f,1.0f);
    osg::ref_ptr<osg::Drawable> cylinder_drawble = new osg::ShapeDrawable(cylinder);
    osgManipulator::setDrawableToAlwaysCull(*cylinder_drawble);
    topgeode->addDrawable(cylinder_drawble);

    _xDragger->addChild(topgeode.get());
    _yDragger->addChild(topgeode.get());
    _zDragger->addChild(topgeode.get());


    //rote
     osg::Quat xrotation ;
     xrotation.makeRotate(osg::Vec3(0.0f,0.0f,1.0f),osg::Vec3(1.0f,0.0f,0.0f));
     _xDragger->setMatrix(osg::Matrix(xrotation));


     osg::Quat yrotation ;
     yrotation.makeRotate(osg::Vec3(0.0f,0.0f,1.0f),osg::Vec3(0.0f,1.0f,0.0f));
     _yDragger->setMatrix(osg::Matrix(yrotation));

     //send different color
     _xDragger->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
     _yDragger->setColor(osg::Vec4(0.0f,1.0f,0.0f,1.0f));
     _zDragger->setColor(osg::Vec4(0.0f,0.0f,1.0f,1.0f));




}
