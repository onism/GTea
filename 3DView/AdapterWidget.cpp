#include "AdapterWidget.h"
//#include "PickHandler.h"
#include "pickdraghandler.h"
#include <QDebug>
#include <osg/PositionAttitudeTransform>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/TerrainManipulator>
AdapterWidget::AdapterWidget( QWidget * parent, const char * name, const QGLWidget * shareWidget, WindowFlags f):
    QGLWidget(parent, shareWidget, f)
{
    _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
    setFocusPolicy(Qt::ClickFocus);
}

void AdapterWidget::init()
{

}

void AdapterWidget::resizeGL( int width, int height )
{
    _gw->getEventQueue()->windowResize(0, 0, width, height );
    _gw->resized(0,0,width,height);
}

void AdapterWidget::keyPressEvent( QKeyEvent* event )
{
    switch(event->key())
        {
        case(Qt::Key_Up):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Up);
            break;
        }
        case(Qt::Key_Down):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Down );
            break;
        }
        case(Qt::Key_Right):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Right );
            break;
        }
        case(Qt::Key_Left):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Left );
            break;
        }
        case(Qt::Key_PageDown):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Page_Down );
            break;
        }
        case(Qt::Key_PageUp):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Page_Up );
            break;
        }
        case(Qt::Key_Alt):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Alt_L );
            break;
        }
        case(Qt::Key_Control):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Control_L );
            break;
        }
        case(Qt::Key_Shift):
        {
            _gw->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KEY_Shift_L );
            break;
        }
        default:
        {
            _gw->getEventQueue()->keyPress( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toLatin1().data() ) );
            break;
        }
        }
}

void AdapterWidget::keyReleaseEvent( QKeyEvent* event )
{
    //odchytavanie udalosti klavesnice
        switch(event->key())
        {
        case(Qt::Key_Up):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Up);
            break;
        }
        case(Qt::Key_Down):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Down );
            break;
        }
        case(Qt::Key_Right):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Right );
            break;
        }
        case(Qt::Key_Left):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Left );
            break;
        }
        case(Qt::Key_PageDown):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Page_Down );
            break;
        }
        case(Qt::Key_PageUp):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Page_Up );
            break;
        }
        case(Qt::Key_Alt):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Alt_L );
            break;
        }
        case(Qt::Key_Control):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Control_L );
            break;
        }
        case(Qt::Key_Shift):
        {
            _gw->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KEY_Shift_L );
            break;
        }

        default:
        {
            _gw->getEventQueue()->keyRelease( (osgGA::GUIEventAdapter::KeySymbol) *(event->text().toLatin1().data() ) );
            break;
        }
        }
}

void AdapterWidget::mousePressEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}


void AdapterWidget::mouseDoubleClickEvent ( QMouseEvent * event )
{
    //odchytavanie udalosti mysi
    int button = 0;
    switch(event->button())
    {
    case(Qt::LeftButton): button = 1; break;
    case(Qt::MidButton): button = 2; break;
    case(Qt::RightButton): button = 3; break;
    case(Qt::NoButton): button = 0; break;
    default: button = 0; break;
    }

    _gw->getEventQueue()->mouseDoubleButtonPress((float)event->x(),(float) event->y(), button);

}


void AdapterWidget::wheelEvent(QWheelEvent *event)
{
    //odchytavanie udalosti mysi
    _gw->getEventQueue()->mouseScroll((event->delta()>0) ?
                                          osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
}

void AdapterWidget::mouseMoveEvent( QMouseEvent* event )
{
    //odchytavanie udalosti mysi
    _gw->getEventQueue()->mouseMotion((float)event->x(),(float) event->y());
}







void AdapterWidget::mouseReleaseEvent( QMouseEvent* event )
{
    int button = 0;
    switch(event->button())
    {
        case(Qt::LeftButton): button = 1; break;
        case(Qt::MidButton): button = 2; break;
        case(Qt::RightButton): button = 3; break;
        case(Qt::NoButton): button = 0; break;
        default: button = 0; break;
    }
    _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}


class AxisCameraUpdateCallback : public osg::NodeCallback
{
public:
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if(nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR)
        {
            osg::Camera* camera = dynamic_cast<osg::Camera*>(node);
            if (camera)
            {
                osg::View* view = camera->getView();
                if (view && view->getNumSlaves() > 0)
                {
                    osg::View::Slave* slave = &view->getSlave(0);
                    if(slave->_camera.get() == camera)
                    {
                        osg::Camera* masterCam = view->getCamera();
                        osg::Vec3 eye, center, up;
                        masterCam->getViewMatrixAsLookAt(eye, center, up, 30);
                        osg::Matrixd matrix;
                         matrix.makeLookAt(eye - center, osg::Vec3(0, 0, 0), up);
                        // matrix.makeLookAt(eye-center, osg::Vec3(0, 0, 0), up); // always look at (0, 0, 0)
                        camera->setViewMatrix(matrix);
                    }
                }
            }
        }
        traverse(node,nv);
    }
};
//////////////////////////////////////////////////////////////////////////
//

ViewerQT::ViewerQT(QWidget * parent, const char * name, const QGLWidget * shareWidget, WindowFlags f):
    AdapterWidget( parent, name, shareWidget, f)
{
    setCamera(this->getCamera());
    // Camera
     getCamera()->setViewport(new osg::Viewport(0,0,width(),height()));
     getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width())/static_cast<double>(height()), 1.0f, 10000.0f);
     getCamera()->setGraphicsContext(getGraphicsWindow());
    getCamera()->setClearColor(osg::Vec4d(51/255.0, 51/255.0, 102/255.0, 0));
    getCamera()->setViewMatrix(osg::Matrix::identity());
//   // getCamera()->setProjectionMatrixAsOrtho2D(0,1280,0,1024);
//   // getCamera()->setClearColor(osg::Vec4d(1, 1, 1, 0));
//    getCamera()->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
//    getCamera()->setClearMask(GL_DEPTH_BUFFER_BIT);
//    getCamera()->setRenderOrder(osg::Camera::POST_RENDER);
//    getCamera()->setUpdateCallback(new AxisCameraUpdateCallback());
//    QString axespath = QCoreApplication::applicationDirPath() + "/axes.osg";
//    osg::Node* axes = osgDB::readNodeFile(axespath.toLatin1().data());
//    qDebug()<<axes->asGroup()->getNumChildren();
//    qDebug()<<axespath;
//    getCamera()->addChild(axes);
    // Trackball
    m_rpTrackball = new osgGA::TrackballManipulator;
  //  setCameraManipulator(m_rpTrackball.get());
    addEventHandler(new osgGA::StateSetManipulator(getCamera()->getOrCreateStateSet()));
   // addEventHandler(new osgViewer::WindowSizeHandler);
    addEventHandler(new osgViewer::StatsHandler);
    // Scene root
    m_rpSceneGroupRoot = new osg::Group;
    setSceneData(m_rpSceneGroupRoot.get());
    osg::Camera* camera = createHUD();
    addSlave(camera, false);
    m_rpSceneGroupRoot->addChild(camera);
//    pickHandler = new PickHandler;
//    addEventHandler(pickHandler);
    addEventHandler(new PickDragHandler());
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator =
            new osgGA::KeySwitchMatrixManipulator;
    keyswitchManipulator->addMatrixManipulator(1,"Trackball",new osgGA::TrackballManipulator());
    keyswitchManipulator->addMatrixManipulator(2,"Flight",new osgGA::FlightManipulator());
    keyswitchManipulator->addMatrixManipulator(3,"Drive",new osgGA::DriveManipulator());
    keyswitchManipulator->addMatrixManipulator(4,"Terrain",new osgGA::TerrainManipulator());
    setCameraManipulator(keyswitchManipulator.get());
    setThreadingModel(osgViewer::Viewer::SingleThreaded);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    _timer.start(10);
}


osg::Camera* ViewerQT::createHUD()
{
    osg::Camera* camera = new osg::Camera;
    camera->setProjectionMatrix(osg::Matrix::ortho2D(-1.5, 17.75, -1.5, 13.86));
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);
    camera->setRenderOrder(osg::Camera::POST_RENDER);
    camera->setUpdateCallback(new AxisCameraUpdateCallback());


    QString axespath = QCoreApplication::applicationDirPath() + "/axes.osg";
    osg::Node *axes = osgDB::readNodeFile(axespath.toLatin1().data());

   camera->addChild(axes);

    return camera;
}


// Reset pespective by bounding nodes
void ViewerQT::ResetCameraPara()
{
    if (m_rpSceneGroupRoot->getNumChildren()==0)
        return;

     osg::BoundingSphere vSphere = m_rpSceneGroupRoot->computeBound();
     setCameraManipulator(m_rpTrackball.get());
    ((osgGA::TrackballManipulator*)getCameraManipulator())->setCenter(vSphere.center());
    ((osgGA::TrackballManipulator*)getCameraManipulator())->setDistance(vSphere.radius()*2.5);
}

// Add model to scene
void ViewerQT::AddModelNode(osg::ref_ptr<osg::Node> mnode)
{
    m_rpSceneGroupRoot->addChild(mnode.get());
}

// Clear model from scene
void ViewerQT::ClearModelNode()
{
    m_rpSceneGroupRoot->removeChildren(0, m_rpSceneGroupRoot->getNumChildren());
}

void ViewerQT::ClearModelNodeByName(osg::ref_ptr<osg::Node> node)
{
    m_rpSceneGroupRoot->removeChild(node);
}
