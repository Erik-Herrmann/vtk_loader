#ifndef CADATAFIELD_H
#define CADATAFIELD_H

#include <QString>
#include <QTextStream>
#include <QList>

class caDataField
{

public:
    explicit caDataField();
    explicit caDataField(QString name);
    virtual ~caDataField();
    void setName(QString name);

    QString getName();
    int numEntries();

    virtual void writeToOut(QTextStream &out) = 0;
    virtual void writeValueToOut(QTextStream &out, int index) = 0;

    virtual QList<int> filterData(int opId, QString valStr) = 0 ;

protected:
    QString m_DataName;
    int m_NumEntries;
};

#endif // CADATAFIELD_H
