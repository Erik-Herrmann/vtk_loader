#ifndef CDATAFIELDINT_H
#define CDATAFIELDINT_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldInt : public caDataField
{
public:
    cDataFieldInt();
    virtual ~cDataFieldInt();

    int* getDataPtr();
    int getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(int *data, int numEntries);

    void filterData(std::set<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(std::set<int> &indices);

private:
    int *m_Data;
};

#endif // CDATAFIELDINT_H
