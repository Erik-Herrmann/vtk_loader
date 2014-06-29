#ifndef CCAMERA_H
#define CCAMERA_H

#include <QVector3D>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

#define GRAD (M_PI/180)

class cCamera
{   
public:
    cCamera(QVector3D position = QVector3D(0.0, 0.0, 0.0));

    void setPosition(QVector3D pos);

    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    void move(QVector3D direction);

    void moveForward(float distance);
    void moveRight(float distance);
    void moveUpward(float distance);

    void reset();

    void render();

private:
    QVector3D m_position;
    QVector3D m_UpVec;
    QVector3D m_RightVec;
    QVector3D m_ViewDir;

    float m_RotX, m_RotY, m_RotZ;

};

#endif // CCAMERA_H
