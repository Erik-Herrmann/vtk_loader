#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QString>
#include <QList>
#include <boost/variant.hpp>

enum MyTypes{
    NOTYPE = -1,
    INT_T = 0,
    UINT_T = 1,
    FLOAT_T = 2,
    DOUBLE_T = 3,
    UCHAR_T = 4,
    VTKPOINT_T = 5

};

//typedef boost::variant<DataField<int>*,
//                       DataField<unsigned int>*,
//                       DataField<float>*,
//                       DataField<double>*,
//                       DataField<unsigned char>*,
//                       DataField<vtkPoint>*
//        > VariantType;

struct vtkPoint{
    vtkPoint(){
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    float x, y, z;
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
/*
    void applyFuncToDataField(DataField<void>* df_v,  void(*func)(void*, int, int, va_list), int numArg, ...){
       va_list vl, vl_this;
       va_start(vl_this, numArg);
       va_copy(vl, vl_this);
        switch (df_v->typeId){
        case INT_T:
            if (df_v->data){
                DataField<int>* df = (DataField<int>*)df_v;
                for (int i=0; i < df->numEntries; ++i){
                        func<int>(df_v, i, numArg, vl);
                }
            }
            break;
        case UINT_T:
            if (df_v->data){
                DataField<unsigned int>* df = (DataField<unsigned int>*)df_v;
                for (int i=0; i < df->numEntries; ++i){
                    func<unsigned int>(df_v, i, numArg, vl);
                }
            }
            break;
        case FLOAT_T:
            if (df_v->data){
                DataField<float>* df = (DataField<float>*)df_v;
                for (int i=0; i < df->numEntries; ++i){
                    func<float>(df_v, i, numArg, vl);
                }
            }
            break;
        case DOUBLE_T:
            if (df_v->data){
                DataField<double>* df = (DataField<double>*)df_v;
                for (int i=0; i < df->numEntries; ++i){
                    func<double>(df_v, i, numArg, vl);
                }
            }
            break;
        case UCHAR_T:
            if (df_v->data){
                DataField<unsigned char>* df = (DataField<unsigned char>*)df_v;
                for (int i=0; i < df->numEntries; ++i){
                    func<unsigned char>(df_v, i, numArg, vl);
                }
            }
            break;
        case VTKPOINT_T:
            if (df_v->data){
                DataField<vtkPoint>* df = (DataField<vtkPoint>*)df_v;
                for (int i=0; i < df->numEntries; ++i){
                    func<vtkPoint>(df_v, i, numArg, vl);
                }
            }
            break;
        default:
            break;
        }
        va_end(vl_this);
    }
    */

    QString filename;
    int numDataFields;
    QList<DataField<void>*> dataFields;
};

#endif // DATASTRUCT_H
