#include "global.h"
#include <QStringList>

//acoustic
    //source
    QString Global::x_acoustic = "";
    QString Global::y_acoustic = "";
    QString Global::z_acoustic = "";
    int Global::p_type = 0;
    QString Global::p1_acoustic = "";
    QString Global::p2_acoustic = "";

    //material
    int Global::material_aco = 0;

    //init
    QString Global::pressure = "";
    QString Global::u_acoustic = "";
    QString Global::v_acoustic = "";
    QString Global::w_acoustic = "";

    //general
    QString Global::time_start_acoustic = "";
    QString Global::time_stop_acoustic = "";
    QString Global::time_step_acoustic = "";

//structure
    //material
    int Global::material_str = 0;

    //initial
    QString Global::x_move_structure = "";
    QString Global::y_move_structure = "";
    QString Global::z_move_structure = "";

    //load
    QString Global::x_structure = "";
    QString Global::y_structure = "";
    QString Global::z_structure = "";
    int Global::load_type = 0;
    QString Global::p1_structure = "";
    QString Global::p2_structure = "";

    //general
    QString Global::time_start_structure = "";
    QString Global::time_stop_structure = "";
    QString Global::time_step_structure = "";

    //listcoupling
    QStringList Global::listcoupling = QStringList();

    //bounding临时变量
    QTreeWidgetItem *Global::bounding =NULL;

Global::Global()
{
}
