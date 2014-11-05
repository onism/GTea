#ifndef SOLUTIONDIALOG_H
#define SOLUTIONDIALOG_H
#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)

#include <vtk/QVTKWidget.h>
#include <vtk/vtkRenderer.h>
#include <vtk/vtkSmartPointer.h>
#include <QDialog>
#include "3DView/corestruct.h"
#include <vtk/vtkVector.h>
#include "3DView/dbcgns.h"
namespace Ui {
class SolutionDialog;
}

class SolutionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SolutionDialog(QWidget *parent = 0);
    ~SolutionDialog();

private slots:
    void on_pushButton_toggled(bool checked);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pushButton_2_toggled(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_3_currentIndexChanged(int index);

private:
    QVTKWidget* vtkWidget;
    Ui::SolutionDialog *ui;
    ROOT* CoreDB ;
    vtkSmartPointer<vtkRenderer> ren;
    int baseindex;
    int zoneindex;
    int sol_index;
    int sol_x;
    int sol_y;
    int sol_z;
    DBCGNS * cgnsIO ;
   // vtkSmartPointer<vtkActor> vectorActor;
};

#endif // SOLUTIONDIALOG_H
