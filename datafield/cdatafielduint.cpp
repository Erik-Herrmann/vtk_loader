#include "cdatafieldint.h"

cDataFieldInt::cDataFieldInt()
    : caDataField()
{
}

unsigned int* cDataFieldInt::getDataPtr(){
    return m_Data;
}

unsigned int cDataFieldInt::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldInt::setData(unsigned int *data, int numEntries){
    if (numEntries > 0)
        m_Data = data;
}

void cDataFieldInt::writeToOut(QTextStream &out){
    if (m_NumEntries)
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldInt::writeValueToOut(QTextStream &out, int index){
    if (index < m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


