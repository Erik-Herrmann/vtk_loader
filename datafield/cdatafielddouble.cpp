#include "cdatafielddouble.h"
#include "helpers.h"
#include <QSet>

cDataFieldDouble::cDataFieldDouble()
    : caDataField()
{
}

cDataFieldDouble::~cDataFieldDouble(){
    delete[] m_Data;
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
    if (numEntries > 0){
        m_Data = data;
        m_NumEntries = numEntries;
    }
}

void cDataFieldDouble::writeToOut(QTextStream &out){
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

void cDataFieldDouble::writeValueToOut(QTextStream &out, int index){
    if (m_NumEntries)
        out << m_DataName << " " << index << ": " << m_Data[index] << "\n";
}

void cDataFieldDouble::filterData(std::set<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        double value = valStr.toDouble();
        // create funktion pointer
        bool (*op)(double&, double&) = 0;
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
                filterList->insert(i);
            }
        }
    }
}

caDataField* cDataFieldDouble::getDatafieldOfListedIndices(std::set<int> &indices){
    if (m_NumEntries){
        double *newData = new double[indices.size()];
        int pos = 0;
        for (int i : indices) {
            newData[pos++] = m_Data[i];
        }
        cDataFieldDouble *newDf = new cDataFieldDouble;
        newDf->setName(m_DataName);
        newDf->setData(newData, indices.size());

        return static_cast<caDataField*>(newDf);
    }
    return 0;
}
