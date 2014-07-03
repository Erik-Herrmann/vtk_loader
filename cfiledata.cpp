#include "cfiledata.h"
#include "datafield/cdatafieldt.h"

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

    GLUquadricObj *gSphere = gluNewQuadric();
    gluQuadricNormals(gSphere, GLU_SMOOTH);
    float mat_emission_detection4[4] = {0.8f, 0.2f, 0.2f, 0.0f};
    float mat_emission[4] = {0.2f, 0.2f, 0.8f, 0.0f};
    float scaleFac = radius/EARTH_RADIUS;
    glEnable(GL_LIGHTING);
    for (int i=0; i < df_p->numEntries(); ++i){
        vtkPoint p = df_p->getValueAt(i);
        float a = df_a->getValueAt(i)*scaleFac;
        if (df_d->getValueAt(i) == 4)
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_emission_detection4);
        else
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_emission);
        glPushMatrix();
        glTranslatef((radius+a)*sin(THETA(p.y))*sin(PHI(p.x)),
                     (radius+a)*cos(THETA(p.y)),
                     (radius+a)*sin(THETA(p.y))*cos(PHI(p.x)));
        gluSphere(gSphere, 0.1f, 6, 6);
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
    gluDeleteQuadric(gSphere);
}


int cFileData::getPointData(std::vector<float> *vertex){
    cDataFieldT<vtkPoint>* df_p =
            static_cast<cDataFieldT<vtkPoint>*>(getDatafield("POINTS"));
    if (!df_p)
        return 0;
    cDataFieldT<float>* df_a =
            static_cast<cDataFieldT<float>*>(getDatafield("altitude"));
    if (!df_a)
        return 0;
    int pos = 0;
    int radius = 10;
    vertex->reserve(df_p->numEntries()*3);
    float scaleFac = 5.0*(float)WORLD_SPHERE_RADIUS/EARTH_RADIUS;
    for (int i=0; i < df_p->numEntries(); ++i){
        vtkPoint p = df_p->getValueAt(i);
        float a = df_a->getValueAt(i)*scaleFac;
        vertex->push_back((radius+a)*sin(THETA(p.y))*sin(PHI(p.x)));
        vertex->push_back((radius+a)*cos(THETA(p.y)));
        vertex->push_back((radius+a)*sin(THETA(p.y))*cos(PHI(p.x)));
        pos+=3;
    }
    return pos;
}

int cFileData::getColorData(std::vector<GLubyte> *color){
    cDataFieldT<unsigned char>* df_d =
            static_cast<cDataFieldT<unsigned char>*>(getDatafield("detection"));
    if (!df_d)
        return 0;

    int pos = 0;
    color->reserve(df_d->numEntries()*3);
    for (int i=0; i < df_d->numEntries(); ++i){
        switch(df_d->getValueAt(i)){
        case 0:
            color->push_back(GLubyte(255));
            color->push_back(GLubyte(255));
            color->push_back(GLubyte(178));
            break;
        case 1:
            color->push_back(GLubyte(254));
            color->push_back(GLubyte(204));
            color->push_back(GLubyte(92));
            break;
        case 2:
            color->push_back(GLubyte(253));
            color->push_back(GLubyte(141));
            color->push_back(GLubyte(60));
            break;
        case 4:
            color->push_back(GLubyte(227));
            color->push_back(GLubyte(26));
            color->push_back(GLubyte(28));
            break;
        }
        pos+=3;
    }
    return pos;
}


int cFileData::getPointColorData(std::vector<float> *vertex, std::vector<GLubyte> *color){
    cDataFieldT<vtkPoint>* df_p =
            static_cast<cDataFieldT<vtkPoint>*>(getDatafield("POINTS"));
    if (!df_p)
        return 0;
    cDataFieldT<float>* df_a =
            static_cast<cDataFieldT<float>*>(getDatafield("altitude"));
    if (!df_a)
        return 0;
    cDataFieldT<unsigned char>* df_d =
            static_cast<cDataFieldT<unsigned char>*>(getDatafield("detection"));
    if (!df_d)
        return 0;

    int pos = 0;
    int radius = 10;
    vertex->reserve(df_p->numEntries()*3);
    float scaleFac = 5.0*(float)WORLD_SPHERE_RADIUS/EARTH_RADIUS;
    for (int i=0; i < df_p->numEntries(); ++i){
        switch(df_d->getValueAt(i)){
        case 0:
            color->push_back(GLubyte(255));
            color->push_back(GLubyte(255));
            color->push_back(GLubyte(178));
            break;
        case 1:
            color->push_back(GLubyte(254));
            color->push_back(GLubyte(204));
            color->push_back(GLubyte(92));
            break;
        case 2:
            color->push_back(GLubyte(253));
            color->push_back(GLubyte(141));
            color->push_back(GLubyte(60));
            break;
        case 4:
            color->push_back(GLubyte(227));
            color->push_back(GLubyte(26));
            color->push_back(GLubyte(28));
            break;
        }
        vtkPoint p = df_p->getValueAt(i);
        float a = df_a->getValueAt(i)*scaleFac;
        vertex->push_back((radius+a)*sin(THETA(p.y))*sin(PHI(p.x)));
        vertex->push_back((radius+a)*cos(THETA(p.y)));
        vertex->push_back((radius+a)*sin(THETA(p.y))*cos(PHI(p.x)));
        pos+=3;
    }
    return pos;
}
