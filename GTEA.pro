#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T13:30:47
#
#-------------------------------------------------

QT       += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GTEA
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    dialog_test.cpp \
    initial_aco.cpp \
    initial_str.cpp \
    sourcedlg.cpp \
    loaddlg.cpp \
    addboundaco.cpp \
    generalaco.cpp \
    generalstr.cpp \
    output.cpp \
    addboudingstr.cpp\
    3DView/AdapterWidget.cpp \
    3DView/cgnslib.c \
    3DView/PickHandler.cpp \
    3DView/buildqttreeview.cpp \
    3DView/TreeViewItem.cpp \
    3DView/loadmsh.cpp \
    3DView/pickdraghandler.cpp \
    3DView/newdragger.cpp \
    3DView/postprocessing.cpp \
    setprodialog.cpp \
    sqlhelper.cpp\
    location.cpp \
    material.cpp \
    modufyprodialog.cpp \
    changepositiondialog.cpp \
    3DView/findnodevistor.cpp \
    3DView/removedata.cpp \
    3DView/db.cpp \
    3DView/dbcgns.cpp \
    3DView/dbcgns_io.cpp \
    3DView/corelib.cpp \
    savefiledialog.cpp



HEADERS  += mainwindow.h \
    dialog.h \
    dialog_test.h \
    initial_aco.h \
    initial_str.h \
    sourcedlg.h \
    loaddlg.h \
    addboundaco.h \
    addboudingstr.h \
    generalaco.h \
    generalstr.h \
    output.h\
    3DView/AdapterWidget.h \
    3DView/cgnslib.h \
    3DView/cgns_struct.h \
    3DView/PickHandler.h \
    3DView/buildqttreeview.h \
    3DView/TreeViewItem.h \
    3DView/loadmsh.h \
    3DView/loadcgns.h \
    3DView/Cgns_solution_struct.h \
    3DView/pickdraghandler.h \
    3DView/newdragger.h \
    3DView/postprocessing.h \
    3DView/InverseDist.h \
    3DView/BaseException.h \
    3DView/Interpolater.h \
    3DView/BaseData.h \
    setprodialog.h \
    sqlhelper.h\
 location.h \
    material.h \
    modufyprodialog.h \
    changepositiondialog.h \
    3DView/findnodevistor.h \
    3DView/removedata.h \
    3DView/db.h \
    3DView/dbcgns.h \
    3DView/dbcgns_io.h \
    3DView/corestruct.h \
    3DView/corelib.h \
    savefiledialog.h


FORMS    += mainwindow.ui \
    dialog.ui \
    dialog_test.ui \
    initial_aco.ui \
    initial_str.ui \
    sourcedlg.ui \
    loaddlg.ui \
    addboundaco.ui \
    addboudingstr.ui \
    generalaco.ui \
    generalstr.ui \
    output.ui \
    setprodialog.ui\
    location.ui \
    material.ui \
    modufyprodialog.ui \
    changepositiondialog.ui \
    savefiledialog.ui


RESOURCES += \
    res.qrc

win32 {
    CONFIG += embed_manifest_exe
    INCLUDEPATH += D:/OSG/include
    CONFIG(debug, debug|release) {
        LIBS+=-LD:/OSG/lib/Debug -losgd -losgQtd -losgGAd -losgTextd -losgShadowd -losgViewerd -losgDBd -losgUtild -losgSimd -losgTerraind -losgManipulatord -losgFXd -losgWidgetd
        OBJECTS_DIR = build/debug
        MOC_DIR = build/debug
        RCC_DIR = build/debug
        DESTDIR = build/debug
    } else {
      LIBS+=-LD:/OSG/lib/Release -losg -losgQt -losgGA -losgText -losgShadow -losgViewer -losgDB -losgUtil -losgSim -losgTerrain -losgManipulator -losgWidget -losgFX
      OBJECTS_DIR = build/release
        MOC_DIR = build/release
        RCC_DIR = build/release
        DESTDIR = build/release
    }
}
