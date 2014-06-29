#ifndef CFILEDATA_H
#define CFILEDATA_H

#include "datafield/cadatafield.h"
#include "datastruct.h"

class cFileData
{
public:
    cFileData();
    ~cFileData();

    caDataField* getDatafield(int i);
    caDataField* getDatafield(QString fieldname);
    QList<caDataField *>& getDatafieldList();

    QString& filename();
    FileTypes fileType();
    int numDataFields();

    void setFilename(QString filename);
    void setFiletype(FileTypes type);
    void push_back(caDataField *df);

    void drawPoints();

private:
    QString m_Filename;
    FileTypes m_FileType;
    int m_NumDataFields;
    QList<caDataField*> m_DataFields;
};

#endif // CFILEDATA_H
