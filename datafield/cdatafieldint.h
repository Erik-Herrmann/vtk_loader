#ifndef CDATAFIELDINT_H
#define CDATAFIELDINT_H

#include "cadatafield.h"
#include "helpers.h"
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

    QList<int> filterData(int opId, QString valStr);

private:
    int *m_Data;
};

#endif // CDATAFIELDINT_H
