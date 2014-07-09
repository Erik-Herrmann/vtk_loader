#ifndef CDATAFIELDT_H
#define CDATAFIELDT_H

#include "cadatafield.h"
#include <QTextStream>
#include "vtkpoint.h"

#define DATAFIELD_ELEMENTS_PER_CHUNK 1000000

template<typename T>
class cDataFieldT : public caDataField
{
public:
    cDataFieldT(unsigned int numElements);
    virtual ~cDataFieldT();

    //std::vector<T> *getDataPtr();
    T getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    //void setData(std::vector<T> *data, int numEntries);

    void filterData(std::set<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(std::set<int> &indices);

    T& operator[](std::size_t index);
    T& at(std::size_t index);
    void push_back(T value);


private:
    unsigned int m_ReservedElements;
    QList<std::vector<T>*> m_DataList;
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
