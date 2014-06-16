#include "cdatafieldvtkpoint.h"
#include "helpers.h"

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

QList<int> cDataFieldVtkPoint::filterData(int opId, QString valStr){
    if (m_NumEntries){
        // create List
        QList<int> nList;
        // get compair value
        float value = valStr.toFloat();
        // create funktion pointer
        bool (*op)(vtkPoint&, float&) = 0;
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
                nList.push_back(i);
            }
        }
        return nList;
    }
    return QList<int>();
}
