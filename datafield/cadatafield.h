#ifndef CADATAFIELD_H
#define CADATAFIELD_H

#include <QString>

class caDataField
{

public:
    explicit caDataField();
    explicit caDataField(QString name);

    QString getName();
    int numEntries();

    virtual void writeToOut(QTextStream &out) = 0;
    virtual void writeValueToOut(QTextStream &out, int index) = 0;

private:
    QString m_DataName;
    int m_NumEntries;
};

#endif // CADATAFIELD_H
