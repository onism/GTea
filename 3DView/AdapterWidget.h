#ifndef ADAPTERWIDGET_H
#define ADAPTERWIDGET_H
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgText/Text>

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QApplication>
#include <QtOpenGL/QGLWidget>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <iostream>
#include "PickHandler.h"
class PickHandle;
using Qt::WindowFlags;

class AdapterWidget  : public QGLWidget
{
public:

    AdapterWidget( QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, WindowFlags f = 0 );

    virtual ~AdapterWidget() {}

    osgViewer::GraphicsWindow* getGraphicsWindow() { return _gw.get(); }
    const osgViewer::GraphicsWindow* getGraphicsWindow() const { return _gw.get(); }

protected:

    void init();
    /**
            *  \fn protected virtual  resizeGL( int width, int height )
            *  \brief
            *  \param width
            *  \param height
            */
        virtual void resizeGL( int width, int height );

    /**
            *  \fn protected virtual  keyPressEvent( QKeyEvent* event )
            *  \brief
            *  \param event
            */
        virtual void keyPressEvent( QKeyEvent* event );

        /**
            *  \fn protected virtual  keyReleaseEvent( QKeyEvent* event )
            *  \brief
            *  \param event
            */
        virtual void keyReleaseEvent( QKeyEvent* event );

        /**
            *  \fn protected virtual  mousePressEvent( QMouseEvent* event )
            *  \brief
            *  \param event
            */
        virtual void mousePressEvent( QMouseEvent* event );

        /**
            *  \fn protected virtual  mouseDoubleClickEvent( QMouseEvent * event )
            *  \brief
            *  \param event
            */
        virtual void mouseDoubleClickEvent ( QMouseEvent * event );

        /**
            *  \fn protected virtual  mouseReleaseEvent( QMouseEvent * event )
            *  \brief
            *  \param [in, out]  event QMouseEvent *
            */
        virtual void mouseReleaseEvent( QMouseEvent* event );

        /**
            *  \fn protected virtual  mouseMoveEvent( QMouseEvent * event )
            *  \brief
            *  \param event
            */
        virtual void mouseMoveEvent( QMouseEvent* event );

        /**
            *  \fn protected virtual  wheelEvent( QWheelEvent * event )
            *  \brief
            *  \param event
            */
        virtual void wheelEvent(QWheelEvent *event);

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
};
class ViewerQT : public osgViewer::Viewer, public AdapterWidget
{
public:

    ViewerQT(QWidget * parent = 0, const char * name = 0, const QGLWidget * shareWidget = 0, WindowFlags f = 0);
    virtual void paintGL()
    {
        frame();
    }

public:
    void ResetCameraPara();
    void AddModelNode(osg::ref_ptr<osg::Node> mnode);
    void ClearModelNode();
    void ClearModelNodeByName(osg::ref_ptr<osg::Node> node);
    osg::Camera*  createHUD();
    osg::Node* createHUD_viewPoint( osgText::Text* text);

public:
    osg::ref_ptr<osgGA::TrackballManipulator>	m_rpTrackball;
    osg::ref_ptr<osg::Group>	m_rpSceneGroupRoot;
    osg::ref_ptr<PickHandler> pickHandler;
    osg::ref_ptr<osgViewer::View> _view;
protected:

    QTimer _timer;
};
#endif // ADAPTERWIDGET_H
