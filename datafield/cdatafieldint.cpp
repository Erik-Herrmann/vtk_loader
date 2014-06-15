#include "cdatafieldint.h"

cDataFieldInt::cDataFieldInt()
    : caDataField()
{
}

cDataFieldInt::~cDataFieldInt(){
    free(m_Data);
}

int* cDataFieldInt::getDataPtr(){
    return m_Data;
}

int cDataFieldInt::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldInt::setData(int *data, int numEntries){
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldInt::writeToOut(QTextStream &out){
    if (m_NumEntries){
        if ((m_DataName == "LINES") || (m_DataName == "POLYGONS")){
            int pos = 0;
            while(pos < m_NumEntries){
                int values = m_Data[pos++];
                out << values;
                for (int i=0; i < values; ++i){
                    out << " " << m_Data[pos++];
                }
                out << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
            }
        }
    }
}

void cDataFieldInt::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


