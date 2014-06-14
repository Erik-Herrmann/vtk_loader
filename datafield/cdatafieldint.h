#ifndef CDATAFIELDINT_H
#define CDATAFIELDINT_H

#include <QTextStream>

class cDataFieldInt : public caDataField
{
public:
    cDataFieldInt();
    int* getDataPtr();
    int getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(int *data, int numEntries);

private:
    int *m_Data;
};

#endif // CDATAFIELDINT_H
