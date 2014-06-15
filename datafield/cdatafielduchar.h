#ifndef CDATAFIELDUCHAR_H
#define CDATAFIELDUCHAR_H

#include "cadatafield.h"
#include <QTextStream>

class cDataFieldUChar : public caDataField
{
public:
    cDataFieldUChar();
    ~cDataFieldUChar();

    unsigned char* getDataPtr();
    unsigned char getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(unsigned char *data, int numEntries);

private:
    unsigned char *m_Data;
};

#endif // CDATAFIELDUCHAR_H
