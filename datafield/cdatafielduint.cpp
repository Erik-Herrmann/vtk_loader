#include "cdatafielduint.h"
#include "helpers.h"

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

QList<int> cDataFieldUInt::filterData(int opId, QString valStr){
    if (m_NumEntries){
        // create List
        QList<int> nList;
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
                nList.push_back(i);
            }
        }
        return nList;
    }
    return QList<int>();
}
