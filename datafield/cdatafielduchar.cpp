#include "cdatafielduchar.h"

cDataFieldUChar::cDataFieldUChar()
    : caDataField()
{
}

unsigned char* cDataFieldUChar::getDataPtr(){
    return m_Data;
}

unsigned char cDataFieldUChar::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldUChar::setData(unsigned char *data, int numEntries){
    if (numEntries > 0)
        m_Data = data;
}

void cDataFieldUChar::writeToOut(QTextStream &out){
    if (m_NumEntries)
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldUChar::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


