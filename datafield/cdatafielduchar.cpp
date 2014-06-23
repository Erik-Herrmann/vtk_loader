#include "cdatafielduchar.h"
#include "helpers.h"
#include <QSet>

cDataFieldUChar::cDataFieldUChar()
    : caDataField()
{
}

cDataFieldUChar::~cDataFieldUChar(){
    free(m_Data);
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
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldUChar::writeToOut(QTextStream &out){
    if (m_NumEntries)
        out << m_DataName << " " << m_NumEntries << "\n";
        for (int i=0; i < m_NumEntries; ++i){
            out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
        }
}

void cDataFieldUChar::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}

void cDataFieldUChar::filterData(QList<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        unsigned char value = (unsigned char)valStr.toUInt();
        // create funktion pointer
        bool (*op)(unsigned char&, unsigned char&) = 0;
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
                filterList->push_back(i);
            }
        }
    }
}


caDataField* cDataFieldUChar::getDatafieldOfListedIndices(QSet<int> &indices){
    if (m_NumEntries){
        unsigned char *newData = (unsigned char*)malloc(indices.count()*sizeof(unsigned char));
        int pos = 0;
        foreach (int i, indices) {
            newData[pos++] = m_Data[i];
        }
        cDataFieldUChar *newDf = new cDataFieldUChar;
        newDf->setName(m_DataName);
        newDf->setData(newData, indices.count());

        return static_cast<caDataField*>(newDf);
    }
    return 0;
}
