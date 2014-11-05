#ifndef GLOBAL_H
#define GLOBAL_H
#include <QObject>
#include <QTreeWidgetItem>

class Global
{
public:
    Global();
//acoustic
    //source
    static QString x_acoustic;
    static QString y_acoustic;
    static QString z_acoustic;
    static int p_type;
    static QString p1_acoustic;
    static QString p2_acoustic;

    //material
    static int material_aco;

    //init
    static QString pressure;
    static QString u_acoustic;
    static QString v_acoustic;
    static QString w_acoustic;

    //general
    static QString time_start_acoustic;
    static QString time_stop_acoustic;
    static QString time_step_acoustic;

//structure
    //material
    static int material_str;

    //initial
    static QString x_move_structure;
    static QString y_move_structure;
    static QString z_move_structure;

    //load
    static QString x_structure;
    static QString y_structure;
    static QString z_structure;
    static int load_type;
    static QString p1_structure;
    static QString p2_structure;

    //general
    static QString time_start_structure;
    static QString time_stop_structure;
    static QString time_step_structure;

    //耦合边界里的第三个list
    static QStringList listcoupling;

    //为了获取bounding下的边界信息，设置一个暂存量
    static QTreeWidgetItem *bounding;



};

#endif // GLOBAL_H
