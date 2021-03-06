#ifndef CDATAFIELDFLOAT_H
#define CDATAFIELDFLOAT_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldFloat : public caDataField
{
public:
    cDataFieldFloat();
    virtual ~cDataFieldFloat();

    float* getDataPtr();
    float getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(float *data, int numEntries);

    void filterData(std::set<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(std::set<int> &indices);

private:
    float *m_Data;
};

#endif // CDATAFIELDFLOAT_H
