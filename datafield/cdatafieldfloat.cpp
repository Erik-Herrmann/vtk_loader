#include "cdatafieldfloat.h"

cDataFieldFloat::cDataFieldFloat()
    : caDataField()
{
}

cDataFieldFloat::~cDataFieldFloat(){
    free(m_Data);
}

float* cDataFieldFloat::getDataPtr(){
    return m_Data;
}

float cDataFieldFloat::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldFloat::setData(float *data, int numEntries){
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldFloat::writeToOut(QTextStream &out){
    if (m_NumEntries)
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldFloat::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


