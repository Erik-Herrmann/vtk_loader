#include "cworldpatch.h"
#include <GL/glu.h>

#define WORLD_PATCHS_RADIUS WORLD_SPHERE_RADIUS*1.6

cWorldPatch::cWorldPatch()
    : m_PatchBegin(WorldCoord()),
      m_PatchEnd(WorldCoord()),
      m_AlphaValue(0.0f),
      m_HistoryValue(0.0f)
{
}

cWorldPatch::cWorldPatch(WorldCoord begin, WorldCoord end)
    : m_PatchBegin(begin),
      m_PatchEnd(end),
      m_AlphaValue(0.0f),
      m_HistoryValue(0.0f)
{
}

cWorldPatch::cWorldPatch(WorldCoord begin, WorldCoord end, int stacks, int slices)
    : m_PatchBegin(begin),
      m_PatchEnd(end),
      m_AlphaValue(0.0f),
      m_HistoryValue(0.0f)
{
    createPatch(stacks, slices);
}

cWorldPatch::~cWorldPatch()
{
    m_PatchPoints.clear();
}

void cWorldPatch::setAlphaValue(float val){
    m_AlphaValue = val;
}

void cWorldPatch::createPatch(int stacks, int slices){
    float longDist = m_PatchEnd.longitude-m_PatchBegin.longitude;
    float latiDist = m_PatchEnd.latitude-m_PatchBegin.latitude;
    float perLong = longDist/stacks;
    float perLati = latiDist/slices;

    for (int i=0; i < stacks; ++i){
        float longi = m_PatchBegin.longitude+i*perLong;
        for (int j=0; j < slices; ++j){
            float lati = m_PatchBegin.latitude+j*perLati;
            Point p(WORLD_PATCHS_RADIUS*sin(THETA(lati))*sin(PHI(longi)),
                    WORLD_PATCHS_RADIUS*cos(THETA(lati)),
                    WORLD_PATCHS_RADIUS*sin(THETA(lati))*cos(PHI(longi)));
            m_PatchPoints.push_back(p);
        }
        Point p(WORLD_PATCHS_RADIUS*sin(THETA(m_PatchEnd.latitude))*sin(PHI(longi)),
                WORLD_PATCHS_RADIUS*cos(THETA(m_PatchEnd.latitude)),
                WORLD_PATCHS_RADIUS*sin(THETA(m_PatchEnd.latitude))*cos(PHI(longi)));
        m_PatchPoints.push_back(p);
    }

    for (int j=0; j < slices; ++j){
        float lati = m_PatchBegin.latitude+j*perLati;
        Point p(WORLD_PATCHS_RADIUS*sin(THETA(lati))*sin(PHI(m_PatchEnd.longitude)),
                WORLD_PATCHS_RADIUS*cos(THETA(lati)),
                WORLD_PATCHS_RADIUS*sin(THETA(lati))*cos(PHI(m_PatchEnd.longitude)));
        m_PatchPoints.push_back(p);
    }
    Point p(WORLD_PATCHS_RADIUS*sin(THETA(m_PatchEnd.latitude))*sin(PHI(m_PatchEnd.longitude)),
            WORLD_PATCHS_RADIUS*cos(THETA(m_PatchEnd.latitude)),
            WORLD_PATCHS_RADIUS*sin(THETA(m_PatchEnd.latitude))*cos(PHI(m_PatchEnd.longitude)));
    m_PatchPoints.push_back(p);

    for (int i=0; i < stacks; ++i){
        for (int j=0; j < slices; ++j){
            Quad q(&m_PatchPoints.at(i+j*(stacks+1)),
                   &m_PatchPoints.at(i+(j+1)*(stacks+1)),
                   &m_PatchPoints.at(i+1+(j+1)*(stacks+1)),
                   &m_PatchPoints.at(i+1+j*(stacks+1)));
            m_PatchQuads.push_back(q);
        }
    }
}

bool cWorldPatch::isInPatch(WorldCoord coord){
    if (m_PatchBegin.longitude > coord.longitude)
        return false;
    if (m_PatchEnd.longitude < coord.longitude)
        return false;

    if (m_PatchBegin.latitude > coord.latitude)
        return false;
    if(m_PatchEnd.latitude < coord.latitude)
        return false;

    return true;
}


void cWorldPatch::drawPatch(){
    glColor4f(1.0f, 0.0f, 0.0f, m_AlphaValue);
    glBegin(GL_QUADS);
    for (Quad q : m_PatchQuads){
        glVertex3fv(q.a()->data());
        glVertex3fv(q.b()->data());
        glVertex3fv(q.c()->data());
        glVertex3fv(q.d()->data());
    }
    glEnd();

    // the test the Patch size
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    glLineWidth(1.5);
    for (Quad q : m_PatchQuads){
        glBegin(GL_LINE_LOOP);
        glVertex3fv(q.a()->data());
        glVertex3fv(q.b()->data());
        glVertex3fv(q.c()->data());
        glVertex3fv(q.d()->data());
        glEnd();
    }
    glLineWidth(1);
}
