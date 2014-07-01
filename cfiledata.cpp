#include "cfiledata.h"
#include "datafield/cdatafieldt.h"
#include <QtOpenGL>



#define EARTH_RADIUS 6371.000

cFileData::cFileData()
    : m_Filename("NOFILE"), m_FileType(UNKNOWN),
      m_NumDataFields(0)
{
}

cFileData::~cFileData(){
    for (QList<caDataField*>::Iterator it = m_DataFields.begin();it != m_DataFields.end(); ++it){
         delete *it;
    }
}

caDataField* cFileData::getDatafield(int i){
    return m_DataFields.at(i);
}

caDataField* cFileData::getDatafield(QString fieldname){
    for (QList<caDataField*>::Iterator it = m_DataFields.begin();it != m_DataFields.end(); ++it){
        if ((*it)->getName() == fieldname){
            return *it;
        }
    }

    return 0;
}

QList<caDataField*>& cFileData::getDatafieldList(){
    return m_DataFields;
}

QString& cFileData::filename(){
    return m_Filename;
}

FileTypes cFileData::fileType(){
    return m_FileType;
}

int cFileData::numDataFields(){
    return m_NumDataFields;
}

void cFileData::setFilename(QString filename){
    m_Filename = filename;
}

void cFileData::setFiletype(FileTypes type){
    m_FileType = type;
}

void cFileData::push_back(caDataField *df){
    m_DataFields.push_back(df);
    m_NumDataFields++;
}

void cFileData::drawPoints(float radius){

    cDataFieldT<vtkPoint>* df_p =
            static_cast<cDataFieldT<vtkPoint>*>(getDatafield("POINTS"));
    if (!df_p)
        return;
    cDataFieldT<float>* df_a =
            static_cast<cDataFieldT<float>*>(getDatafield("altitude"));
    if (!df_a)
        return;
    cDataFieldT<unsigned char>* df_d =
            static_cast<cDataFieldT<unsigned char>*>(getDatafield("detection"));
    if (!df_d)
        return;

    float scaleFac = radius/EARTH_RADIUS;
    glPushMatrix();
    glPointSize(2);
    glBegin(GL_POINTS);
    for (int i=0; i < df_p->numEntries(); ++i){
        vtkPoint p = df_p->getValueAt(i);
        float a = df_a->getValueAt(i)*scaleFac;
        if (df_d->getValueAt(i) == 4)
            glColor3f(1.0, 0.0, 0.0);
        else
            glColor3f(0.0, 0.0, 0.8);

        glVertex3f((radius+a)*sin(p.x)*cos(p.y),
                   (radius+a)*sin(p.x)*sin(p.y),
                   (radius+a)*cos(p.x));
    }
    glEnd();
    glPopMatrix();
    glPointSize(1);
}


int cFileData::getPointData(float *data){
    cDataFieldT<vtkPoint>* df_p =
            static_cast<cDataFieldT<vtkPoint>*>(getDatafield("POINTS"));
    if (!df_p)
        return 0;
    cDataFieldT<float>* df_a =
            static_cast<cDataFieldT<float>*>(getDatafield("altitude"));
    if (!df_a)
        return 0;

    int radius = 10;
    data = new float[df_p->numEntries()*3];
    float scaleFac = (float)radius/EARTH_RADIUS;
    int pos = 0;
    for (int i=0; i < df_p->numEntries(); ++i){
        vtkPoint p = df_p->getValueAt(i);
        float a = df_a->getValueAt(i)*scaleFac;
        data[pos++] = (radius+a)*sin(p.x)*cos(p.y);
        data[pos++] = (radius+a)*sin(p.x)*sin(p.y);
        data[pos++] = (radius+a)*cos(p.x);
    }
    return pos;
}
