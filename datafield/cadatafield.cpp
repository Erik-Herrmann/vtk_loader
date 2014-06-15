#include "cadatafield.h"

caDataField::caDataField()
    : m_DataName(""), m_NumEntries(0)
{
}

caDataField::caDataField(QString name)
    : m_DataName(name), m_NumEntries(0)
{
}

void caDataField::setName(QString name){
    m_DataName = name;
}

QString caDataField::getName(){
    return m_DataName;
}

int caDataField::numEntries(){
    return m_NumEntries;
}
