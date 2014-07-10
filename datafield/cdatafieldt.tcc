#include "cdatafieldt.h"
#include "helpers.h"
#include <set>

template<typename T>
cDataFieldT<T>::cDataFieldT(unsigned int numElements)
    : caDataField(),
      m_ReservedElements(numElements)
{
    unsigned int vecs = numElements / DATAFIELD_ELEMENTS_PER_CHUNK;
    unsigned int lastVecElems = numElements % DATAFIELD_ELEMENTS_PER_CHUNK;
    for (;vecs > 0; --vecs){
        std::vector<T> *vec = new std::vector<T>;
        vec->reserve(DATAFIELD_ELEMENTS_PER_CHUNK);
        m_DataList.push_back(vec);
    }
    if (lastVecElems){
        std::vector<T> *vec = new std::vector<T>;
        vec->reserve(lastVecElems);
        m_DataList.push_back(vec);
    }
}

template<typename T>
cDataFieldT<T>::~cDataFieldT(){
    foreach (std::vector<T>* vec, m_DataList) {
        vec->clear();
        delete vec;
    }
    m_DataList.clear();
}

/*
 *  template<typename T>
 *  std::vector<T>* cDataFieldT<T>::getDataPtr(){
 *      return m_DataList;
 *  }
*/

template<typename T>
T cDataFieldT<T>::getValueAt(int index){
    if (index < m_NumEntries)
        return at(index);
    else
        return T();
}

/*
 *  template<typename T>
 *  void cDataFieldT<T>::setData(std::vector<T> *data, int numEntries){
 *      if (numEntries > 0){
 *          m_DataList = data;
 *          m_NumEntries = numEntries;
 *      }
 *  }
*/

template<typename T>
void cDataFieldT<T>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << at(i) << "\n";
        }
    }
}


template<typename T>
void cDataFieldT<T>::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << at(index) << "\n";
}

template<typename T>
T& cDataFieldT<T>::operator[](std::size_t index){
    return at(index);
}

template<typename T>
T& cDataFieldT<T>::at(std::size_t index){
    if (index >= m_NumEntries)
        return m_DataList.back()->back();
    unsigned int listEntry = index / DATAFIELD_ELEMENTS_PER_CHUNK;
    unsigned int vecEntry = index % DATAFIELD_ELEMENTS_PER_CHUNK;

    return m_DataList.at(listEntry)->at(vecEntry);
}

template<typename T>
void cDataFieldT<T>::push_back(T value){
    if (m_NumEntries >= m_ReservedElements)
        return;

    unsigned int listEntry = m_NumEntries / DATAFIELD_ELEMENTS_PER_CHUNK;
    m_DataList.at(listEntry)->push_back(value);
    m_NumEntries++;
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
            if(op(at(i), value)){
                filterList->insert(i);
            }
        }
    }
}


template<typename T>
caDataField* cDataFieldT<T>::getDatafieldOfListedIndices(std::set<int> &indices){
    if (m_NumEntries && !indices.empty()){
        cDataFieldT<T> *newDf = new cDataFieldT<T>(indices.size());
        newDf->setName(m_DataName);
        for (int i : indices) {
            newDf->push_back(at(i));
        }
        return static_cast<caDataField*>(newDf);
    }
    return 0;
}


template<typename T>
std::vector<T>* cDataFieldT<T>::getDataVector(){
    std::vector<T>* vec = new std::vector<T>;
    vec->reserve(m_NumEntries);
    foreach (std::vector<T>* v, m_DataList) {
        vec->insert(vec->end(), v->begin(), v->end());
    }
    return vec;
}
