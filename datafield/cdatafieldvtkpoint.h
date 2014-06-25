#ifndef CDATAFIELDVTKPOINT_H
#define CDATAFIELDVTKPOINT_H

#include "cadatafield.h"
#include "vtkpoint.h"
#include <QTextStream>


class cDataFieldVtkPoint : public caDataField
{
public:
    cDataFieldVtkPoint();
    virtual ~cDataFieldVtkPoint();

    vtkPoint* getDataPtr();
    vtkPoint getValueAt(int index);
    void writeToOut(QTextStream &out);
    void writeValueToOut(QTextStream &out, int index);

    void setData(vtkPoint *data, int numEntries);

    void filterData(std::set<int> *filterList, int opId, QString valStr);
    caDataField* getDatafieldOfListedIndices(std::set<int> &indices);

private:
    vtkPoint *m_Data;
};

#endif // CDATAFIELDVTKPOINT_H
