#ifndef LOADMSH_H
#define LOADMSH_H
#include <QTextStream>
#include <osg/Group>
typedef struct {
    float x;
    float y;
    float z;
}MshNodeData;


typedef struct {
    QList<MshNodeData> data;
    int first_index;
    int last_index;
    int zone_id;
    int nd;
    int type;
}MshNode;//index 10
typedef struct{
    int first_index;
    int last_index;
    int zone_id;
    int nd;
    int type;
    int element_type;

}MshCell;//index 12
typedef struct{
    int *vertics;
    int cl;
    int cr;
    int type;
}FaceData;
typedef struct{
    int first_index;
    int last_index;
    int zone_id;
    int nd;
    int type;
    int element_type;
    QList<FaceData> facedatas;

}MshFaces;//13

typedef struct{
    int zone_id;
    char type[512];
    char name[512];
}Name_Id;


class LoadMsh
{
public:
    LoadMsh(QString filename);
     osg::ref_ptr<osg::Group> oroot;
      QList<Name_Id> Face_Names;
      QList<QString> face_wall_names;
private:
    QList<MshNodeData> read_zones(int data_numbers,QTextStream &stream,int dim);
    QList<FaceData> read_face(int data_numbers, QTextStream &stream, int elem_type);
    int openfile(QString filename);
    int drawmsh();
    QList<MshNode> zone_nodes;
    QList<MshFaces> faces_nodes;
  //  QList<Name_Id> Face_Names;
    MshNode getMshNode_by_zone_id(int zone_id);
    int getFaceNameById(int zone_id,char* buf);

};

#endif // LOADMSH_H
