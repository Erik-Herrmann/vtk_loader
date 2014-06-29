#include "ccamera.h"
#include <GL/glu.h>

cCamera::cCamera(QVector3D position)
    : m_position(position),
      m_UpVec(QVector3D(0.0, 1.0, 0.0)),
      m_RightVec(QVector3D(1.0, 0.0, 0.0)),
      m_ViewDir(QVector3D(0.0, 0.0, -1.0)),
      m_RotX(0.0), m_RotY(0.0), m_RotZ(0.0)
{
}

void cCamera::setPosition(QVector3D pos){
    m_position = pos;
}

void cCamera::move(QVector3D direction){
    m_position += direction;
}

void cCamera::rotateX(float angle){

    m_RotX += angle;

    m_ViewDir = (m_ViewDir * cos(angle*GRAD) +
                m_UpVec * sin(angle*GRAD))
                .normalized();

    m_UpVec = QVector3D::crossProduct(m_ViewDir, m_RightVec)*-1;
}

void cCamera::rotateY(float angle){

    m_RotY += angle;

    m_ViewDir = (m_ViewDir * cos(angle*GRAD) -
                m_RightVec * sin(angle*GRAD))
                .normalized();

    m_RightVec = QVector3D::crossProduct(m_ViewDir, m_UpVec);
}

void cCamera::rotateZ(float angle){

    m_RotZ += angle;

    m_RightVec = (m_RightVec * cos(angle*GRAD) +
                  m_UpVec * sin(angle*GRAD))
                .normalized();

    m_UpVec = QVector3D::crossProduct(m_ViewDir, m_RightVec)*-1;
}

void cCamera::moveForward(float distance){
    m_position += m_ViewDir*(distance);
}

void cCamera::moveRight(float distance){
    m_position += m_RightVec*distance;
}

void cCamera::moveUpward(float distance){
    m_position += m_UpVec*distance;
}

void cCamera::reset(){
    m_position = QVector3D(0.0, 0.0, 0.0);
    m_UpVec = QVector3D(0.0, 1.0, 0.0);
    m_RightVec = QVector3D(1.0, 0.0, 0.0);
    m_ViewDir = QVector3D(0.0, 0.0, -1.0);
    m_RotX = m_RotY = m_RotZ = 0.0;
}

void cCamera::render(){

    QVector3D viewPoint = m_position+m_ViewDir;

    gluLookAt(m_position.x(), m_position.y(), m_position.z(),
              viewPoint.x(), viewPoint.y(), viewPoint.z(),
              m_UpVec.x(), m_UpVec.y(), m_UpVec.z() );
}
