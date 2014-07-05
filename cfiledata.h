#ifndef CFILEDATA_H
#define CFILEDATA_H

#include "datafield/cadatafield.h"
#include "datastruct.h"
#include "globaldefines.h"
#include <QtOpenGL>
#include <GL/glu.h>

//-------------------------------------------------------
//  Conversion londitude, latitude, altitude -> x, y, z
//    x = alti*sin(THETA(i))*sin(PHI(k))
//    y = alti*cos(THETA(i))
//    z = alti*sin(THETA(i))*cos(PHI(k))
//-------------------------------------------------------

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

    void drawPoints(float radius);
    GLuint getDisplayList();
    int getPointData(std::vector<float> *vertex);
    int getColorData(std::vector<GLubyte> *color);
    int getPointColorData(std::vector<float> *vertex, std::vector<GLubyte> *color);

private:
    QString m_Filename;
    FileTypes m_FileType;
    int m_NumDataFields;
    QList<caDataField*> m_DataFields;
};

#endif // CFILEDATA_H
