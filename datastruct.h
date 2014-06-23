#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QString>
#include <QList>
#include <QDateTime>
#include "vtkpoint.h"
#include "datafield/datafieldincludes.h"

enum MyTypes{
    NOTYPE = -1,
    INT_T = 0,
    UINT_T = 1,
    FLOAT_T = 2,
    DOUBLE_T = 3,
    UCHAR_T = 4,
    VTKPOINT_T = 5

};

enum FileTypes{
    UNKNOWN = -1,
    MIPAS = 0,
    AIRS = 1,
    CLAMS = 2,
    TROPO = 3
};

struct PolyLine{
    vtkPoint point;
    QDateTime time;
};

struct FileData{
    FileData(){
        filename = "NOFILE";
        fileType = UNKNOWN;
        numDataFields = 0;
    }

    ~FileData(){
        for (QList<caDataField*>::Iterator it = dataFields.begin();it != dataFields.end(); ++it){
             delete *it;
        }
    }

    caDataField* getDatafield(int i){
        return dataFields.at(i);
    }

    caDataField* getDatafield(QString fieldname){
        for (QList<caDataField*>::Iterator it = dataFields.begin();it != dataFields.end(); ++it){
            if ((*it)->getName() == fieldname){
                return *it;
            }
        }

        return 0;
    }

    QString filename;
    FileTypes fileType;
    int numDataFields;
    QList<caDataField*> dataFields;
};

#endif // DATASTRUCT_H
