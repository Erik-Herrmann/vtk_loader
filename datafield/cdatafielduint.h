#ifndef CDATAFIELDUINT_H
#define CDATAFIELDUINT_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldUInt : public caDataField
{
public:
    cDataFieldUInt();
    virtual ~cDataFieldUInt();

    unsigned int* getDataPtr();
    unsigned int getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(unsigned int *data, int numEntries);

    void filterData(QList<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(QSet<int> &indices);

private:
    unsigned int *m_Data;
};

#endif // CDATAFIELDUINT_H
