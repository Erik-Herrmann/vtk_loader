#ifndef CDATAFIELDDOUBLE_H
#define CDATAFIELDDOUBLE_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldDouble : public caDataField
{
public:
    cDataFieldDouble();
    virtual ~cDataFieldDouble();

    double* getDataPtr();
    double getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(double *data, int numEntries);

    void filterData(std::set<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(std::set<int> &indices);

private:
    double *m_Data;
};

#endif // CDATAFIELDDOUBLE_H
