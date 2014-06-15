#ifndef CDATAFIELDINT_H
#define CDATAFIELDINT_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldDouble : public caDataField
{
public:
    cDataFieldDouble();
    ~cDataFieldDouble();

    double* getDataPtr();
    double getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(double *data, int numEntries);

private:
    double *m_Data;
};

#endif // CDATAFIELDINT_H
