#include "cdatafieldfloat.h"
#include "helpers.h"
#include <QSet>

cDataFieldFloat::cDataFieldFloat()
    : caDataField()
{
}

cDataFieldFloat::~cDataFieldFloat(){
    delete[] m_Data;
}

float* cDataFieldFloat::getDataPtr(){
    return m_Data;
}

float cDataFieldFloat::getValueAt(int index){
    if (index < m_NumEntries)
        return m_Data[index];
    else
        return 0;
}

void cDataFieldFloat::setData(float *data, int numEntries){
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldFloat::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        if (m_DataName == "time"){
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << " = \t"
                    << Helpers::juliantimeToDatetime(m_Data[i]).toString("dddd dd.MMMM yyyy hh:mm:ss")
                    << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
            }
        }
    }
}

void cDataFieldFloat::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}


void cDataFieldFloat::filterData(QList<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        float value = valStr.toFloat();
        // create funktion pointer
        bool (*op)(float&, float&) = 0;
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


caDataField* cDataFieldFloat::getDatafieldOfListedIndices(QSet<int> &indices){
    if (m_NumEntries){
        float *newData = new float[indices.count()];
        int pos = 0;
        foreach (int i, indices) {
            newData[pos++] = m_Data[i];
        }
        cDataFieldFloat *newDf = new cDataFieldFloat;
        newDf->setName(m_DataName);
        newDf->setData(newData, indices.count());

        return static_cast<caDataField*>(newDf);
    }
    return 0;
}

