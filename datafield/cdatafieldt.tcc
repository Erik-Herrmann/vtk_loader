#include "cdatafieldt.h"
#include "helpers.h"
#include <set>

template<typename T>
cDataFieldT<T>::cDataFieldT()
    : caDataField()
{
}

template<typename T>
cDataFieldT<T>::~cDataFieldT(){
    delete m_Data;
}

template<typename T>
std::vector<T>* cDataFieldT<T>::getDataPtr(){
    return m_Data;
}

template<typename T>
T cDataFieldT<T>::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data->at(index);
    else
        return T();
}

template<typename T>
void cDataFieldT<T>::setData(std::vector<T> *data, int numEntries){
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

template<typename T>
void cDataFieldT<T>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data->at(i) << "\n";
        }
    }
}


template<typename T>
void cDataFieldT<T>::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data->at(index) << "\n";
}



template<typename T>
void cDataFieldT<T>::filterData(std::set<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        T value = valStr.toDouble();
        // create funktion pointer
        bool (*op)(T&, T&) = 0;
        // set funktion to pointer by opId
        switch (opId) {
        case 0:
            op = &Helpers::equal_to;
            break;
        case 1:
            op = &Helpers::not_equal_to;
            break;
        case 2:
            op = &Helpers::greater;
            break;
        case 3:
            op = &Helpers::greater_equal;
            break;
        case 4:
            op = &Helpers::less;
            break;
        case 5:
            op = &Helpers::less_equal;
            break;
        }
        // compair and update List
        for (int i=0; i < m_NumEntries; ++i){
            if(op(m_Data->at(i), value)){
                filterList->insert(i);
            }
        }
    }
}


template<typename T>
caDataField* cDataFieldT<T>::getDatafieldOfListedIndices(std::set<int> &indices){
    if (m_NumEntries && !indices.empty()){
        std::vector<T> *newData = new std::vector<T>;
        newData->reserve(indices.size());
        for (int i : indices) {
            newData->push_back(m_Data->at(i));
        }
        cDataFieldT<T> *newDf = new cDataFieldT<T>;
        newDf->setName(m_DataName);
        newDf->setData(newData, newData->size());

        return static_cast<caDataField*>(newDf);
    }
    return 0;
}
