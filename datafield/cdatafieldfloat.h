#ifndef CDATAFIELDINT_H
#define CDATAFIELDINT_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldfloat : public caDataField
{
public:
    cDataFieldfloat();
    float* getDataPtr();
    float getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(float *data, int numEntries);

private:
    float *m_Data;
};

#endif // CDATAFIELDINT_H
