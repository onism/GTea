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
    global.cpp \
    materialstr.cpp \
    coupling.cpp \
    changepositiondialog.cpp \
    3DView/findnodevistor.cpp \
    3DView/removedata.cpp \
    3DView/db.cpp \
    3DView/dbcgns.cpp \
    3DView/dbcgns_io.cpp \
    savefiledialog.cpp \
    3DView/corelib.c \
    solutiondialog.cpp \
    3DView/cgnsIO.c \
    displayform.cpp




HEADERS  += mainwindow.h \
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
    global.h \
    materialstr.h \
    coupling.h\
    changepositiondialog.h \
    3DView/findnodevistor.h \
    3DView/removedata.h \
    3DView/db.h \
    3DView/dbcgns.h \
    3DView/corestruct.h \
    3DView/corelib.h \
    savefiledialog.h \
    3DView/CGNSIO.h \
    solutiondialog.h \
    displayform.h



FORMS    += mainwindow.ui \
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
    materialstr.ui \
    coupling.ui \
    changepositiondialog.ui \
    savefiledialog.ui \
    solutiondialog.ui \
    displayform.ui



RESOURCES += \
    res.qrc

win32 {
    CONFIG += embed_manifest_exe
    INCLUDEPATH += D:/OSG/include
    CONFIG(debug, debug|release) {
        LIBS+=-LD:/OSG/lib/Debug -losgd -losgQtd -losgGAd -losgTextd -losgShadowd -losgViewerd -losgDBd -losgUtild -losgSimd -losgTerraind -losgManipulatord -losgFXd -losgWidgetd   -lvtkalglib-6.1 -lvtkChartsCore-6.1 -lvtkCommonColor-6.1 -lvtkCommonComputationalGeometry-6.1 -lvtkCommonCore-6.1 -lvtkCommonDataModel-6.1 -lvtkCommonExecutionModel-6.1 -lvtkCommonMath-6.1 -lvtkCommonMisc-6.1 -lvtkCommonSystem-6.1 -lvtkCommonTransforms-6.1 -lvtkDICOMParser-6.1 -lvtkDomainsChemistry-6.1 -lvtkexoIIc-6.1 -lvtkexpat-6.1  -lvtkFiltersCore-6.1 -lvtkFiltersExtraction-6.1 -lvtkFiltersFlowPaths-6.1 -lvtkFiltersGeneral-6.1 -lvtkFiltersGeneric-6.1 -lvtkFiltersGeometry-6.1 -lvtkFiltersHybrid-6.1 -lvtkFiltersHyperTree-6.1 -lvtkFiltersImaging-6.1 -lvtkFiltersModeling-6.1  -lvtkFiltersProgrammable-6.1  -lvtkFiltersSources-6.1 -lvtkFiltersStatistics-6.1 -lvtkFiltersTexture-6.1 -lvtkFiltersVerdict-6.1 -lvtkfreetype-6.1 -lvtkftgl-6.1 -lvtkGeovisCore-6.1 -lvtkgl2ps-6.1 -lvtkGUISupportQt-6.1 -lvtkGUISupportQtOpenGL-6.1 -lvtkGUISupportQtSQL-6.1  -lvtkImagingColor-6.1 -lvtkImagingCore-6.1  -lvtkImagingGeneral-6.1 -lvtkImagingHybrid-6.1 -lvtkImagingMath-6.1    -lvtkInfovisCore-6.1 -lvtkInfovisLayout-6.1  -lvtkInteractionStyle-6.1 -lvtkInteractionWidgets-6.1      -lvtkRenderingAnnotation-6.1 -lvtkRenderingContext2D-6.1 -lvtkRenderingCore-6.1 -lvtkRenderingFreeType-6.1 -lvtkRenderingFreeTypeOpenGL-6.1   -lvtkRenderingLabel-6.1  -lvtkRenderingOpenGL-6.1 -lvtkRenderingQt-6.1 -lvtkRenderingVolume-6.1  -lvtkRenderingVolumeOpenGL-6.1  -lvtkViewsQt-6.1
        OBJECTS_DIR = build/debug
        MOC_DIR = build/debug
        RCC_DIR = build/debug
        DESTDIR = build/debug
    } else {
      LIBS+=-LD:/OSG/lib/Release -losg -losgQt -losgGA -losgText -losgShadow -losgViewer -losgDB -losgUtil -losgSim -losgTerrain -losgManipulator -losgWidget -losgFX -lvtkalglib-6.1 -lvtkChartsCore-6.1 -lvtkCommonColor-6.1 -lvtkCommonComputationalGeometry-6.1 -lvtkCommonCore-6.1 -lvtkCommonDataModel-6.1 -lvtkCommonExecutionModel-6.1 -lvtkCommonMath-6.1 -lvtkCommonMisc-6.1 -lvtkCommonSystem-6.1 -lvtkCommonTransforms-6.1 -lvtkDICOMParser-6.1 -lvtkDomainsChemistry-6.1 -lvtkexoIIc-6.1 -lvtkexpat-6.1  -lvtkFiltersCore-6.1 -lvtkFiltersExtraction-6.1 -lvtkFiltersFlowPaths-6.1 -lvtkFiltersGeneral-6.1 -lvtkFiltersGeneric-6.1 -lvtkFiltersGeometry-6.1 -lvtkFiltersHybrid-6.1 -lvtkFiltersHyperTree-6.1 -lvtkFiltersImaging-6.1 -lvtkFiltersModeling-6.1  -lvtkFiltersProgrammable-6.1  -lvtkFiltersSources-6.1 -lvtkFiltersStatistics-6.1 -lvtkFiltersTexture-6.1 -lvtkFiltersVerdict-6.1 -lvtkfreetype-6.1 -lvtkftgl-6.1 -lvtkGeovisCore-6.1 -lvtkgl2ps-6.1 -lvtkGUISupportQt-6.1 -lvtkGUISupportQtOpenGL-6.1 -lvtkGUISupportQtSQL-6.1  -lvtkImagingColor-6.1 -lvtkImagingCore-6.1  -lvtkImagingGeneral-6.1 -lvtkImagingHybrid-6.1 -lvtkImagingMath-6.1    -lvtkInfovisCore-6.1 -lvtkInfovisLayout-6.1  -lvtkInteractionStyle-6.1 -lvtkInteractionWidgets-6.1      -lvtkRenderingAnnotation-6.1 -lvtkRenderingContext2D-6.1 -lvtkRenderingCore-6.1 -lvtkRenderingFreeType-6.1 -lvtkRenderingFreeTypeOpenGL-6.1   -lvtkRenderingLabel-6.1  -lvtkRenderingOpenGL-6.1 -lvtkRenderingQt-6.1 -lvtkRenderingVolume-6.1  -lvtkRenderingVolumeOpenGL-6.1  -lvtkViewsQt-6.1
        OBJECTS_DIR = build/debug
      OBJECTS_DIR = build/release
        MOC_DIR = build/release
        RCC_DIR = build/release
        DESTDIR = build/release
    }
}
