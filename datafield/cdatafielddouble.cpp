#include "cdatafielddouble.h"

cDataFieldDouble::cDataFieldDouble()
    : caDataField()
{
}

double* cDataFieldDouble::getDataPtr(){
    return m_Data;
}

double cDataFieldDouble::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldDouble::setData(double *data, int numEntries){
    if (numEntries > 0)
        m_Data = data;
}

void cDataFieldDouble::writeToOut(QTextStream &out){
    if (m_NumEntries)
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldDouble::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


