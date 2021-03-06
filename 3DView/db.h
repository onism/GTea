/*******************************************************************************
 * File:        DB.h
 * Author:      Ashish Gupta
 * Revision:    4
 ******************************************************************************/


#ifndef _DB_H
#define	_DB_H

#include "corestruct.h"
#include <QString>
#define DB_OK       0
#define DB_ERROR    1

#define DB_READ     1
#define DB_MODIFY   2
#define DB_WRITE    3

class DB {
protected :
    //! Attributes
    ROOT* CoreDB;
    QString inputGFile;
    QString inputPFile;
    QString inputSFile;
    QString outputGFile;
    QString outputPFile;
    QString outputSFile;
    unsigned int outputGIOMode;
    unsigned int outputPIOMode;
    unsigned int outputSIOMode;
    unsigned int parent;
    unsigned int state;
public:
    //! Constructors and Distructors
    DB();
    DB(const DB& other);
    virtual ~DB();
    //! Operations
    void Set_DB(ROOT* object);
    ROOT* Get_DB() const;
    unsigned int isParent() const;
    unsigned int getState() const;
    void Reset();
    DB& operator=(const DB& other);
    void Set_InputGrid_Filename(QString filename);
    void Set_InputParam_Filename(QString  filename);
    void Set_InputSolution_Filename(QString filename);
    void Set_OutputGrid_Filename(QString filename);
    void Set_OutputParam_Filename(QString filename);
    void Set_OutputSolution_Filename(QString filename);
    QString Get_InputGrid_Filename() const;
    QString Get_InputParam_Filename() const;
    QString Get_InputSolution_Filename() const;
    QString Get_OutputGrid_Filename() const;
    QString Get_OutputParam_Filename() const;
    QString Get_OutputSolution_Filename() const;
    void Set_OutputGrid_IOMode(int mode);
    void Set_OutputParam_IOMode(int mode);
    void Set_OutputSolution_IOMode(int mode);
    int Get_OuputGrid_IOMode() const;
    int Get_OuputParam_IOMode() const;
    int Get_OutputSolution_IOMode() const;
    virtual void Read_DB()=0;
    virtual void Write_DB()=0;
    void Copy(const ROOT* object);
protected :
    void Reset_DB();
};

#endif	/* _DB_H */

