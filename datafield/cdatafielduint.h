#ifndef CDATAFIELDINT_H
#define CDATAFIELDINT_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldUInt : public caDataField
{
public:
    cDataFieldUInt();
    ~cDataFieldUInt();

    unsigned int* getDataPtr();
    unsigned int getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(unsigned int *data, int numEntries);

private:
    unsigned int *m_Data;
};

#endif // CDATAFIELDINT_H
