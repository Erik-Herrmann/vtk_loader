#include "cdatafieldvtkpoint.h"

cDataFieldVtkPoint::cDataFieldVtkPoint()
    : caDataField()
{
}

cDataFieldVtkPoint::~cDataFieldVtkPoint(){
    free(m_Data);
}

vtkPoint* cDataFieldVtkPoint::getDataPtr(){
    return m_Data;
}

vtkPoint cDataFieldVtkPoint::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return vtkPoint();
}

void cDataFieldVtkPoint::setData(vtkPoint *data, int numEntries){
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldVtkPoint::writeToOut(QTextStream &out){
    if (m_NumEntries){
        for (int i=0; i < m_NumEntries; ++i){
            vtkPoint &p = m_Data[i];
            out << m_DataName << " " << i << ": " << p.x << " " << p.y << " " << p.z << "\n";
        }
    }
}

void cDataFieldVtkPoint::writeValueToOut(QTextStream &out, int index){
    if (index < m_NumEntries){
        vtkPoint &p = m_Data[index];
        out << m_DataName << " " << index << ": " << p.x << " " << p.y << " " << p.z << "\n";
    }
}


