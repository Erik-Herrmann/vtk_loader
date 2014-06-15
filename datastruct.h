#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QString>
#include <QList>
#include "vtkpoint.h"

enum MyTypes{
    NOTYPE = -1,
    INT_T = 0,
    UINT_T = 1,
    FLOAT_T = 2,
    DOUBLE_T = 3,
    UCHAR_T = 4,
    VTKPOINT_T = 5

};

template<typename T>
struct DataField{
    DataField(){
        dataname = "NONE";
        numEntries = 0;
        typeId = NOTYPE;
    }
    ~DataField(){
        free(data);
    }
    int getTypeSizeFromId(MyTypes Id){
        switch (Id) {
        case INT_T:
            return sizeof(int);
        case UINT_T:
            return sizeof(unsigned int);
        case FLOAT_T:
            return sizeof(float);
        case DOUBLE_T:
            return sizeof(double);
        case UCHAR_T:
            return sizeof(unsigned char);
        case VTKPOINT_T:
            return sizeof(vtkPoint);
        default:
            return 0;
        }
    }

    QString dataname;
    int numEntries;
    MyTypes typeId;
    int typeSize;
    T *data;
};


struct FileData{
    FileData(){
        filename = "NOFILE";
        numDataFields = 0;
    }

    ~FileData(){
        for (QList<DataField<void>*>::Iterator it = dataFields.begin();it != dataFields.end(); ++it){
             delete *it;
        }
    }

    DataField<void>* getDatafield(int i){
        return dataFields.at(i);
    }

    DataField<void>* getDatafield(QString fieldname){
        for (QList<DataField<void>*>::Iterator it = dataFields.begin();it != dataFields.end(); ++it){
            if ((*it)->dataname == fieldname){
                return *it;
            }
        }

        return 0;
    }

    QString filename;
    int numDataFields;
    QList<DataField<void>*> dataFields;
};

#endif // DATASTRUCT_H
