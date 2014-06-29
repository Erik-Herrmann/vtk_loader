#ifndef CDATAFIELDT_H
#define CDATAFIELDT_H

#include "cadatafield.h"
#include <QTextStream>
#include "vtkpoint.h"

template<typename T>
class cDataFieldT : public caDataField
{
public:
    cDataFieldT();
    virtual ~cDataFieldT();

    T* getDataPtr();
    T getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(T *data, int numEntries);

    void filterData(std::set<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(std::set<int> &indices);

private:
    T *m_Data;
};

template<>
void cDataFieldT<double>::writeToOut(QTextStream &out);

template<>
void cDataFieldT<float>::writeToOut(QTextStream &out);

template<>
void cDataFieldT<int>::writeToOut(QTextStream &out);

template<>
void cDataFieldT<vtkPoint>::writeToOut(QTextStream &out);

template<>
void cDataFieldT<vtkPoint>::writeValueToOut(QTextStream &out, int index);

template<>
void cDataFieldT<int>::filterData(std::set<int> *filterList, int opId, QString valStr);

template<>
void cDataFieldT<vtkPoint>::filterData(std::set<int> *filterList, int opId, QString valStr);

template<>
caDataField* cDataFieldT<int>::getDatafieldOfListedIndices(std::set<int> &indices);


#include "cdatafieldt.tcc"

#endif // CDATAFIELDT_H
