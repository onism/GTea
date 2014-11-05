#include "solutiondialog.h"
#include "ui_solutiondialog.h"
#include "3DView/dbcgns.h"
#include "3DView/corelib.h"
#include <QFileDialog>

#include <vtk/vtkArrowSource.h>
#include <vtk/vtkCellArray.h>
#include <vtk/vtkGlyph2D.h>
#include <vtk/vtkPointData.h>
#include <vtk/vtkImageData.h>
#include <vtk/vtkImageSliceMapper.h>
#include <vtk/vtkImageSlice.h>
#include <vtk/vtkInteractorStyleImage.h>
#include <vtk/vtkPolyDataMapper.h>
#include <vtk/vtkPolyData.h>
#include <vtk/vtkPoints.h>
#include <vtk/vtkRenderer.h>
#include <vtk/vtkRenderWindow.h>
#include <vtk/vtkRenderWindowInteractor.h>
#include <vtk/vtkSmartPointer.h>
#include <vtk/vtkXMLPolyDataWriter.h>
#include <vtk/vtkVersion.h>
#include <vtk/vtkUnstructuredGrid.h>
#include <vtk/vtkPolyVertex.h>
#include <vtk/vtkDataSetMapper.h>
#include <vtk/vtkProperty.h>
#include <vtk/vtkDoubleArray.h>
#include <vtk/vtkPolyhedron.h>
#include <vtk/vtkPointLocator.h>
#include <QDebug>
#include <vtk/vtkContourFilter.h>
#include <vtk/vtkGaussianSplatter.h>
#include <vtk/vtkAxes.h>
#include <vtk/vtkTubeFilter.h>
#include <vtk/vtkStripper.h>
#include <vtk/vtkLookupTable.h>
#include <vtk/vtkDelaunay3D.h>
#include <vtk/vtkDelaunay2D.h>
#include <vtk/vtkLabeledDataMapper.h>
#include <vtk/vtkActor2D.h>
#include <vtk/vtkGlyph3D.h>
#include <vtk/vtkCleanPolyData.h>
#include <vtk/vtkPlaneSource.h>
#include <vtk/vtkMath.h>

#include <vtk/vtkBandedPolyDataContourFilter.h>
#include <vtk/vtkNamedColors.h>
#include <vtk/vtkScalarBarActor.h>
#include <vtk/vtkColorTransferFunction.h>
#include <vtk/vtkSmoothPolyDataFilter.h>
#include <vtk/vtkPolyDataNormals.h>
#include <vtk/vtkTextProperty.h>
#define VTK_NEW(type, instance); vtkSmartPointer<type> instance = vtkSmartPointer<type>::New();
SolutionDialog::SolutionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SolutionDialog)
{
    ui->setupUi(this);
    CoreDB = NULL;
    baseindex = 0;
    zoneindex = 0;
    sol_x     = -1;
    sol_y     = -1;
    sol_z     = -1;
    sol_index = 0;
    cgnsIO = NULL;
    vtkWidget = new QVTKWidget(this,QFlag(0));
    ui->verticalLayout->addWidget(vtkWidget);
    //ui->verticalLayout->update();
    ren = vtkSmartPointer<vtkRenderer>::New();
    ren->SetBackground(0.0,0.0,0.0);
    vtkWidget->GetRenderWindow()->AddRenderer(ren);
    //vectorActor = vtkSmartPointer<vtkActor>::New();
}

SolutionDialog::~SolutionDialog()
{

    CoreDB = NULL;
   // ren->Delete();
    delete cgnsIO;
    cgnsIO = NULL;
   delete vtkWidget;
    delete ui;
}

void SolutionDialog::on_pushButton_toggled(bool checked)
{


}

void SolutionDialog::on_comboBox_currentIndexChanged(int index)
{

    ui->comboBox_2->clear();
    if(index > -1){
        for(int i=0;i<CoreDB->bases[index].nzones;i++)
        {
            ui->comboBox_2->addItem(QString::fromLatin1(CoreDB->bases[index].zones[i].name));
        }
        baseindex = index;
    }

}

void SolutionDialog::on_comboBox_2_currentIndexChanged(int index)
{
    ui->comboBox_3->clear();
    if(index > -1){
    for(int i=0;i<CoreDB->bases[baseindex].zones[index].nsols;i++)
    {
        ui->comboBox_3->addItem(QString::fromLatin1(CoreDB->bases[baseindex].zones[index].sols[i].name));
    }
    zoneindex = index;
    }
}


void SolutionDialog::on_pushButton_2_toggled(bool checked)
{

}

void SolutionDialog::on_pushButton_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open File"),  ".", tr("  Files (*.cgns)"));
    if(!filepath.isEmpty()){
//        ui->cob_z->clear();
//        ui->cob_x->clear();
//        ui->cob_y->clear();
//        ui->comboBox_3->clear();
//        ui->comboBox_2->clear();
        ui->comboBox->clear();
    if(cgnsIO == NULL){
        cgnsIO = new DBCGNS;

    }else{
CoreDB = NULL;
cgnsIO->Reset();

    }

    cgnsIO->Set_InputGrid_Filename(filepath) ;
qDebug()<<"2";
    cgnsIO->Read_DB();
    qDebug()<<"1";
      CoreDB = cgnsIO->Get_DB();
      qDebug()<<"1";
    int basenum = CoreDB->nbases;
    qDebug()<<"1";

  //
qDebug()<<QString::fromLatin1(CoreDB->bases[0].name);
    for(int i=0;i<basenum;i++)
    {
        ui->comboBox->addItem(QString::fromLatin1(CoreDB->bases[i].name));
    }

    if(CoreDB->bases[0].celldim==2){
        //the z should set unabled
        ui->cob_z->setVisible(false);
        ui->label_6->setVisible(false);
    }
    }
    //delete cgnsIO;
   // cgnsIO = NULL;
}
//vector field
void SolutionDialog::on_pushButton_2_clicked()
{
    if(ui->cob_x->currentIndex()==-1&&ui->cob_y->currentIndex()==-1&&ui->cob_z->currentIndex()==-1)
    {
        return;
    }
    qDebug()<<ui->cob_x->currentText()<<ui->cob_y->currentText();
        int dim = CoreDB->bases[baseindex].celldim;
         int coorddim = 0;
        if(CoreDB->bases[baseindex].zones[zoneindex].type == 2){
            coorddim = CoreDB->bases[baseindex].zones[zoneindex].dim[0]*CoreDB->bases[baseindex].zones[zoneindex].dim[1]*CoreDB->bases[baseindex].zones[zoneindex].dim[2];
        }else
        {
            coorddim = CoreDB->bases[baseindex].zones[zoneindex].dim[0];
        }
        vtkSmartPointer<vtkPoints> points =
                vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkDoubleArray> pointDataArray =
            vtkSmartPointer<vtkDoubleArray>::New();
          pointDataArray->Initialize();

        if(dim==2){
            sol_x = ui->cob_x->currentIndex();
            sol_y = ui->cob_y->currentIndex();
            int count = 0;
            if(sol_x != -1)
                count++;
            if(sol_y != -1)
                count++;
            if(count == 1){
               if(sol_x==-1){
                   sol_x = sol_y;
               }
            }
             pointDataArray->SetNumberOfComponents(count);
              for(int i=0;i<coorddim;i++)
              {
                  double x =CoreDB->bases[baseindex].zones[zoneindex].verts[i].x;
                  double y =CoreDB->bases[baseindex].zones[zoneindex].verts[i].y;
                  double xy[2];
                  xy[0] = x;
                  xy[1] = y;
                  points->InsertNextPoint(xy);
                  if(count == 1)
                  {
                     pointDataArray->InsertNextValue(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i]);
                  }else if(count == 2)
                  {
                     pointDataArray->InsertNextTuple2(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_y].data[i]);
                  }else{
                      return ;
                  }
              }
              //set salars



        }else if(dim==3)
        {
            sol_x = ui->cob_x->currentIndex();
            sol_y = ui->cob_y->currentIndex();
            sol_z = ui->cob_z->currentIndex();
            int count = 0;
            if(sol_x != -1)
                count++;
            if(sol_y != -1)
                count++;
            if(sol_z != -1)
                count++;

            pointDataArray->SetNumberOfComponents(count);
            for(int i=0;i<coorddim;i++)
            {
                double x =CoreDB->bases[baseindex].zones[zoneindex].verts[i].x;
                double y =CoreDB->bases[baseindex].zones[zoneindex].verts[i].y;
                double z =CoreDB->bases[baseindex].zones[zoneindex].verts[i].z;
                points->InsertNextPoint(x,y,z);

                if(count == 1)
                {
                   pointDataArray->InsertNextValue(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i]);
                }else if(count == 2)
                {
                   pointDataArray->InsertNextTuple2(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_y].data[i]);
                }else if(count == 3)
                {
                    pointDataArray->InsertNextTuple3(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_y].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_z].data[i]);

                }else{
                    return ;
                }
            }

        }

       vtkSmartPointer<vtkPolyVertex> polyvertex = vtkSmartPointer<vtkPolyVertex>::New();
        polyvertex->GetPointIds()->SetNumberOfIds(coorddim);
        int i=0;
        for(i=0;i<coorddim;i++)
        {
           polyvertex->GetPointIds()->SetId(i,i);
        }
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();

        grid->SetPoints(points);
//        grid->InsertNextCell(polyvertex->GetCellType(),
//                  polyvertex->GetPointIds());

       grid->GetPointData()->SetScalars(pointDataArray);
   //grid->GetPointData()->SetVectors(pointDataArray);
   double range[2];

   grid->GetScalarRange(range);
  int numPoints    = grid->GetNumberOfPoints();
     VTK_NEW(vtkLookupTable,lookUpTable);
     lookUpTable->SetRampToLinear();
     lookUpTable->SetNumberOfColors( 256 );
     lookUpTable->SetHueRange(0.667,0.0);
     lookUpTable->SetRange(range);
     lookUpTable->Build();
   vtkSmartPointer<vtkArrowSource> arrowSource = vtkSmartPointer<vtkArrowSource>::New();
   arrowSource->Update();
   VTK_NEW(vtkScalarBarActor, colorbar);
   colorbar->SetTitle("color");
   vtkSmartPointer<vtkTextProperty> prop = colorbar->GetLabelTextProperty ();
     prop->SetFontSize (4);
   colorbar->SetTitleTextProperty(prop);
   colorbar->SetOrientationToVertical();
   colorbar->SetPosition(0.8,0.1);
   colorbar->SetPosition2(0.9,0.9);
   colorbar->SetWidth(0.2);
   colorbar->SetNumberOfLabels(6);
   colorbar->SetLookupTable(lookUpTable);
   if(dim == 2){

//          VTK_NEW(vtkLookupTable,lut);

//          lut->SetNumberOfTableValues(numPoints);
//          std::vector<double> norm;
//          double min(std::numeric_limits<double>::max()), max(0);

//          for(int i=0;i<numPoints;i++)
//          {
//            // Color coding with the eigenvector
//            double vect[3];
//            data->GetPointData()->GetVectors()->GetTuple(i,vect);
//            norm.push_back(vtkMath::Norm(vect));
//            if(norm[i] < min)
//                min = norm[i];

//            if(norm[i] > max)
//                max = norm[i];
//          }

//          for(int i=0;i<numPoints;i++)
//          {


//            double hue =   (2.0/3.0)/(min-max) * norm[i] + (2 * max / 3)/(max-min);

//            double hsv[3] = {hue, 1, 1};
//            double rgb[3];
//            vtkMath::HSVToRGB(hsv, rgb);

//            lut->SetTableValue(i, rgb[0], rgb[1], rgb[2]);
//            this->ValueArray->SetTuple1(i, (unsigned int)i);
//          }

//          data->GetPointData()->SetScalars( this->ValueArray );

//          this->Glyph->SetColorModeToColorByScalar();

//          this->Glyph->Modified();

//          this->Mapper->SetLookupTable(lut);
//          this->Mapper->SetScalarRange(0, numPoints);









       vtkSmartPointer<vtkDelaunay2D> delaunay
           = vtkSmartPointer<vtkDelaunay2D>::New();
        delaunay->SetTolerance(0.00001);
        delaunay->SetInputData(grid);

        delaunay->Update();
         qDebug()<<range[0]<<range[1];
       vtkSmartPointer<vtkGlyph2D> glyphFilter = vtkSmartPointer<vtkGlyph2D>::New();
       glyphFilter->SetSourceConnection(arrowSource->GetOutputPort());
       glyphFilter->OrientOn();
       glyphFilter->SetVectorModeToUseVector();
      // glyphFilter->SetScaleModeToScaleByScalar();
        //  glyphFilter->SetColorModeToColorByScalar();
       glyphFilter->ScalingOn();
      glyphFilter->SetVectorModeToUseVector();
      glyphFilter->SetScaleModeToScaleByVector();
       glyphFilter->SetScaleFactor(0.20);
       glyphFilter->SetInputData(grid);
       glyphFilter->SetVectorModeToUseNormal();
       glyphFilter->SetScaleModeToDataScalingOff();
       glyphFilter->GeneratePointIdsOn();
      //glyphFilter->SetInputConnection(delaunay->GetOutputPort());
       //glyphFilter->SetInputData(delaunay->GetOutput());
       glyphFilter->Update();
       vtkSmartPointer<vtkPolyDataMapper> vectorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
       vectorMapper->SetInputConnection(glyphFilter->GetOutputPort());
       vectorMapper->SetLookupTable(lookUpTable);
       vectorMapper->SetScalarRange(range);
       vectorMapper->SetScalarModeToUsePointData();
        vectorMapper->SetColorModeToMapScalars();
       vectorMapper->SetScalarVisibility(1);
       vectorMapper->UseLookupTableScalarRangeOn();

       vectorMapper->Update();

       vtkSmartPointer<vtkActor> vectorActor = vtkSmartPointer<vtkActor>::New();
       vectorActor->SetMapper(vectorMapper);
       ren->RemoveAllViewProps();
       ren->AddViewProp(vectorActor);
       ren->AddActor(colorbar);
       ren->ResetCamera();
       vtkWidget->update();
       ui->verticalLayout->update();

   }else{
       vtkSmartPointer<vtkGlyph3D> glyphFilter = vtkSmartPointer<vtkGlyph3D>::New();
       glyphFilter->SetSourceConnection(arrowSource->GetOutputPort());
       glyphFilter->OrientOn();
       glyphFilter->SetVectorModeToUseVector();
        glyphFilter->SetScaleModeToScaleByScalar();
        glyphFilter->SetColorModeToColorByScalar();
       glyphFilter->ScalingOn();
       glyphFilter->SetVectorModeToUseVector();
       glyphFilter->SetScaleModeToScaleByVector();
       glyphFilter->SetScaleFactor(0.50);
       glyphFilter->SetInputData(grid);

       glyphFilter->Update();
       vtkSmartPointer<vtkPolyDataMapper> vectorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
       vectorMapper->SetInputConnection(glyphFilter->GetOutputPort());
       vtkSmartPointer<vtkActor> vectorActor = vtkSmartPointer<vtkActor>::New();
       vectorActor->SetMapper(vectorMapper);
       ren->RemoveAllViewProps();
       ren->AddViewProp(vectorActor);
       ren->ResetCamera();
       vtkWidget->update();
       ui->verticalLayout->update();
   }









return ;

}

void SolutionDialog::on_pushButton_3_clicked()
{

    if(ui->cob_x->currentIndex()==-1&&ui->cob_y->currentIndex()==-1&&ui->cob_z->currentIndex()==-1)
    {
        return;
    }
    qDebug()<<ui->cob_x->currentText()<<ui->cob_y->currentText();
        int dim = CoreDB->bases[baseindex].celldim;
         int coorddim = 0;
        if(CoreDB->bases[baseindex].zones[zoneindex].type == 2){
            coorddim = CoreDB->bases[baseindex].zones[zoneindex].dim[0]*CoreDB->bases[baseindex].zones[zoneindex].dim[1]*CoreDB->bases[baseindex].zones[zoneindex].dim[2];
        }else
        {
            coorddim = CoreDB->bases[baseindex].zones[zoneindex].dim[0];
        }
        vtkSmartPointer<vtkPoints> points =
                vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkDoubleArray> pointDataArray =
            vtkSmartPointer<vtkDoubleArray>::New();
          pointDataArray->Initialize();

        if(dim==2){
            sol_x = ui->cob_x->currentIndex();
            sol_y = ui->cob_y->currentIndex();
            int count = 0;
            if(sol_x != -1)
                count++;
            if(sol_y != -1)
                count++;
            if(count == 1){
               if(sol_x==-1){
                   sol_x = sol_y;
               }
            }
             pointDataArray->SetNumberOfComponents(count);
              for(int i=0;i<coorddim;i++)
              {
                  double x =CoreDB->bases[baseindex].zones[zoneindex].verts[i].x;
                  double y =CoreDB->bases[baseindex].zones[zoneindex].verts[i].y;
                  double xy[2];
                  xy[0] = x;
                  xy[1] = y;
                  points->InsertNextPoint(xy);
                  if(count == 1)
                  {
                     pointDataArray->InsertNextValue(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i]);
                  }else if(count == 2)
                  {
                     pointDataArray->InsertNextTuple2(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_y].data[i]);
                  }else{
                      return ;
                  }
              }
              //set salars



        }else if(dim==3)
        {
            sol_x = ui->cob_x->currentIndex();
            sol_y = ui->cob_y->currentIndex();
            sol_z = ui->cob_z->currentIndex();
            int count = 0;
            if(sol_x != -1)
                count++;
            if(sol_y != -1)
                count++;
            if(sol_z != -1)
                count++;

            pointDataArray->SetNumberOfComponents(count);
            for(int i=0;i<coorddim;i++)
            {
                double x =CoreDB->bases[baseindex].zones[zoneindex].verts[i].x;
                double y =CoreDB->bases[baseindex].zones[zoneindex].verts[i].y;
                double z =CoreDB->bases[baseindex].zones[zoneindex].verts[i].z;
                points->InsertNextPoint(x,y,z);

                if(count == 1)
                {
                   pointDataArray->InsertNextValue(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i]);
                }else if(count == 2)
                {
                   pointDataArray->InsertNextTuple2(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_y].data[i]);
                }else if(count == 3)
                {
                    pointDataArray->InsertNextTuple3(CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_x].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_y].data[i],CoreDB->bases[baseindex].zones[zoneindex].sols[sol_index].flds[sol_z].data[i]);

                }else{
                    return ;
                }
            }

        }

       vtkSmartPointer<vtkPolyVertex> polyvertex = vtkSmartPointer<vtkPolyVertex>::New();
        polyvertex->GetPointIds()->SetNumberOfIds(coorddim);
        int i=0;
        for(i=0;i<coorddim;i++)
        {
           polyvertex->GetPointIds()->SetId(i,i);
        }
        vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();

        grid->SetPoints(points);
//        grid->InsertNextCell(polyvertex->GetCellType(),
//                  polyvertex->GetPointIds());

       grid->GetPointData()->SetScalars(pointDataArray);

     if(dim==2){
         vtkSmartPointer<vtkDelaunay2D> delaunay
             = vtkSmartPointer<vtkDelaunay2D>::New();
          delaunay->SetTolerance(0.00001);
          delaunay->SetInputData(grid);

          delaunay->Update();
          double range[2];
          grid->GetScalarRange(range);
          qDebug()<<range[0]<<range[1];
          vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
          lut->SetRampToLinear();
          lut->SetNumberOfColors( 256 );
          lut->SetHueRange(0.667,0.0);
          lut->SetRange(range);
          lut->Build();
//          lut->SetNumberOfTableValues(6);
//          lut->SetRange(range);
//          lut->Build();
//          vtkSmartPointer<vtkNamedColors> namedColors =
//              vtkSmartPointer<vtkNamedColors>::New();
//       //
//       //  lut->SetHueRange(0.667,0.0);


////          lut->SetNumberOfColors(6);

//            double rgba[4];
//                    // Test setting and getting colors here.
//                   namedColors->GetColor("Red",rgba);
//                    namedColors->SetColor("My Red",rgba);
//                    namedColors->GetColor("My Red",rgba);
//                    lut->SetTableValue(0,rgba);
//                    namedColors->GetColor("DarkGreen",rgba);
//                   lut->SetTableValue(1,rgba);

//                   // Alternatively we can use tuple methods here:
//                  lut->SetTableValue(2,namedColors->GetColor4d("Blue").GetData());
//                   lut->SetTableValue(3,namedColors->GetColor4d("Cyan").GetData());
//                    lut->SetTableValue(4,namedColors->GetColor4d("Magenta").GetData());
//                   lut->SetTableValue(5,namedColors->GetColor4d("Yellow").GetData());
//                  //  lut->SetTableValue(6,namedColors->GetColor4d("White").GetData());





                   VTK_NEW(vtkSmoothPolyDataFilter, smoothFilter);
                      smoothFilter->SetInputConnection(delaunay->GetOutputPort());
                      smoothFilter->SetNumberOfIterations(100);

                      //Produces smoother normals for the surface
                      VTK_NEW(vtkPolyDataNormals, normalsFilter);
                      normalsFilter->SetInputConnection(smoothFilter->GetOutputPort());
                      normalsFilter->ComputeCellNormalsOn();
                      normalsFilter->SetFeatureAngle(80);




          vtkSmartPointer<vtkCleanPolyData> filledContours =
              vtkSmartPointer<vtkCleanPolyData>::New();
           filledContours->SetInputConnection(delaunay->GetOutputPort());

           vtkSmartPointer<vtkPolyDataMapper> contourMapper =
              vtkSmartPointer<vtkPolyDataMapper>::New();
          //  contourMapper->SetInputConnection(filledContours->GetOutputPort());
            contourMapper->SetInputConnection(normalsFilter->GetOutputPort());
            contourMapper->SetScalarRange(range[0], range[1]);


             contourMapper->Update();


            vtkSmartPointer<vtkActor> contourActor =
               vtkSmartPointer<vtkActor>::New();
             contourActor->SetMapper(contourMapper);
             contourActor->GetProperty()->SetInterpolationToFlat();


         vtkSmartPointer<vtkContourFilter> popSurface =
           vtkSmartPointer<vtkContourFilter>::New();
         popSurface->SetInputConnection(normalsFilter->GetOutputPort());

           popSurface->GenerateValues(5, .99*range[0], .99*range[1]);

           popSurface->ComputeScalarsOn();
           popSurface->ComputeGradientsOff();





           vtkSmartPointer<vtkPolyDataMapper> vectorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
           vectorMapper->SetInputConnection(popSurface->GetOutputPort());

           vectorMapper->SetScalarRange(range);
            vectorMapper->SetLookupTable(lut);

           vectorMapper->SetScalarVisibility(1);
           vectorMapper->UseLookupTableScalarRangeOn();
           vectorMapper->SetColorModeToMapScalars();
           vectorMapper->Update();
           vtkSmartPointer<vtkActor> vectorActor = vtkSmartPointer<vtkActor>::New();
           vectorActor->SetMapper(vectorMapper);
           vectorActor->GetProperty()->SetInterpolationToFlat();



           VTK_NEW(vtkScalarBarActor, colorbar);
           colorbar->SetTitle("color");
           vtkSmartPointer<vtkTextProperty> prop = colorbar->GetLabelTextProperty ();
             prop->SetFontSize (8);
           colorbar->SetTitleTextProperty(prop);
                   colorbar->SetOrientationToVertical();
                   colorbar->SetPosition(0.8,0.1);
                   colorbar->SetPosition2(0.9,0.9);
                   colorbar->SetWidth(0.2);
                   colorbar->SetNumberOfLabels(6);
                   colorbar->SetLookupTable(lut);


           ren->RemoveAllViewProps();
           ren->AddViewProp(vectorActor);
          // ren->AddViewProp(contourActor);
           ren->AddActor2D(colorbar);
           //ren->AddViewProp(colorbar);
           ren->ResetCamera();
           vtkWidget->update();
           ui->verticalLayout->update();
     }else if(dim == 3)
     {
         vtkSmartPointer<vtkDelaunay3D> delaunay
             = vtkSmartPointer<vtkDelaunay3D>::New();

          delaunay->SetInputData(grid);

          delaunay->Update();


         vtkSmartPointer<vtkContourFilter> popSurface =
           vtkSmartPointer<vtkContourFilter>::New();
           popSurface->SetInputConnection(delaunay->GetOutputPort());
           double range[2];
           grid->GetScalarRange(range);

           vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();

           lut->SetRange(range);
           lut->SetNumberOfTableValues(6);
           lut->Build();
           popSurface->GenerateValues(5, range[0], range[1]);

           popSurface->ComputeScalarsOff();
           popSurface->ComputeGradientsOff();


           vtkSmartPointer<vtkPolyDataMapper> vectorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
            vectorMapper->SetInputConnection(popSurface->GetOutputPort());
          //  vectorMapper->SetInputData(grid);
vectorMapper->SetScalarModeToUsePointFieldData();
vectorMapper->ColorByArrayComponent(0,0);
           vectorMapper->SetScalarRange(range);
           vectorMapper->SetLookupTable(lut);
           vectorMapper->SetScalarModeToUseCellData();
           vectorMapper->SetScalarVisibility(1);
           vtkSmartPointer<vtkActor> vectorActor = vtkSmartPointer<vtkActor>::New();
           vectorActor->SetMapper(vectorMapper);
           vectorActor->GetProperty()->SetInterpolationToFlat();
           ren->RemoveAllViewProps();
           ren->AddViewProp(vectorActor);
           ren->ResetCamera();
           vtkWidget->update();
           ui->verticalLayout->update();

     }



return ;

}

void SolutionDialog::on_pushButton_4_clicked()
{



}

void SolutionDialog::on_comboBox_3_currentIndexChanged(int index)
{

    //filled the flds by sols
    ui->cob_x->clear();
    ui->cob_y->clear();
    ui->cob_z->clear();
    if(index > -1){
    int nflds = CoreDB->bases[baseindex].zones[zoneindex].sols[index].nflds;
    for(int i = 0; i < nflds; i++)
    {
        ui->cob_x->addItem(QString::fromLatin1(CoreDB->bases[baseindex].zones[zoneindex].sols[index].flds[i].name));
        ui->cob_y->addItem(QString::fromLatin1(CoreDB->bases[baseindex].zones[zoneindex].sols[index].flds[i].name));
        ui->cob_z->addItem(QString::fromLatin1(CoreDB->bases[baseindex].zones[zoneindex].sols[index].flds[i].name));
    }
    ui->cob_x->setCurrentIndex(-1);
    ui->cob_y->setCurrentIndex(-1);
    ui->cob_z->setCurrentIndex(-1);
    sol_index = index;
    }
}
