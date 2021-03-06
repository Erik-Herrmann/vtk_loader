#include "cdatafielduint.h"
#include "helpers.h"
#include <QSet>

cDataFieldUInt::cDataFieldUInt()
    : caDataField()
{
}

cDataFieldUInt::~cDataFieldUInt(){
    delete[] m_Data;
}

unsigned int* cDataFieldUInt::getDataPtr(){
    return m_Data;
}

unsigned int cDataFieldUInt::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldUInt::setData(unsigned int *data, int numEntries){
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldUInt::writeToOut(QTextStream &out){
    if (m_NumEntries)
        out << m_DataName << " " << m_NumEntries << "\n";
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldUInt::writeValueToOut(QTextStream &out, int index){
    if (index < m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}

void cDataFieldUInt::filterData(std::set<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        unsigned int value = valStr.toUInt();
        // create funktion pointer
        bool (*op)(unsigned int&, unsigned int&) = 0;
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
            if(op(m_Data[i], value)){
                filterList->insert(i);
            }
        }
    }
}


caDataField* cDataFieldUInt::getDatafieldOfListedIndices(std::set<int> &indices){
    if (m_NumEntries){
        unsigned int *newData = new unsigned int[indices.size()];
        int pos = 0;
        for (int i : indices) {
            newData[pos++] = m_Data[i];
        }
        cDataFieldUInt *newDf = new cDataFieldUInt;
        newDf->setName(m_DataName);
        newDf->setData(newData, indices.size());

        return static_cast<caDataField*>(newDf);
    }
    return 0;
}
