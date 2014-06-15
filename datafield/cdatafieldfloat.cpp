#include "cdatafieldfloat.h"

cDataFieldfloat::cDataFieldfloat()
    : caDataField()
{
}

cDataFieldfloat::~cDataFieldfloat(){
    free(m_Data);
}

float* cDataFieldfloat::getDataPtr(){
    return m_Data;
}

float cDataFieldfloat::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldfloat::setData(float *data, int numEntries){
    if (numEntries > 0)
        m_Data = data;
}

void cDataFieldfloat::writeToOut(QTextStream &out){
    if (m_NumEntries)
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldfloat::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


