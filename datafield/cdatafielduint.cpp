#include "cdatafielduint.h"

cDataFieldUInt::cDataFieldUInt()
    : caDataField()
{
}

cDataFieldUInt::~cDataFieldUInt(){
    free(m_Data);
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
    if (numEntries > 0)
        m_Data = data;
}

void cDataFieldUInt::writeToOut(QTextStream &out){
    if (m_NumEntries)
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldUInt::writeValueToOut(QTextStream &out, int index){
    if (index < m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


