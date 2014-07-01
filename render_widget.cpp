#include "render_widget.h"

#include <QtGui>
//#include <QtOpenGL>
#include <GL/glu.h>
#include <QPixmap>
#include <QImage>
#include <math.h>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

render_widget::render_widget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SingleBuffer | QGL::DepthBuffer | QGL::StencilBuffer ), parent)
{
    setWindowTitle("Render Widget");
    m_Camera.setPosition(QVector3D(0.0, 0.0, 20.0));
    texture = 0;
}

render_widget::~render_widget()
{
}

QSize render_widget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize render_widget::sizeHint() const
{
    return QSize(800, 600);
}

void render_widget::addToDrawlist(cFileData *data){
    drawList.push_back(data);

//    float *pointData = 0;
//    int size = data->getPointData(pointData);
//    cDrawObject<float> *obj = new cDrawObject<float>(this->context()->functions(),
//                                                     pointData, size, GL_POINTS);
//    filedataDrawindexMap[data] = drawList.size();
//    drawList.push_back(obj);
//    delete pointData;

    update();
}

void render_widget::removeFromDrawlist(cFileData *data){
    drawList.removeAll(data);

//    drawList.removeAt(filedataDrawindexMap[data]);

    update();
}

void render_widget::loadTextures(){    
    QPixmap pixMap(QCoreApplication::applicationDirPath() + "/texture/earth_8k.jpg");
    if (pixMap.isNull())
        return;
    tex = QGLWidget::convertToGLFormat(pixMap.toImage());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(),
                 tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 tex.bits());
}

void render_widget::initializeGL()
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    // load world texture to GPU-Buffer
    loadTextures();

    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_TEXTURE_2D);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void drawAxis(){
    glLineWidth(5);

    // x axis
    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(5.0, 0.0, 0.0);
    glEnd();

    // y axis
    glBegin(GL_LINES);
        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 5.0, 0.0);
    glEnd();

    // z axis
    glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 5.0);
    glEnd();

    glLineWidth(1);
}

void render_widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw coordinate axis
    glLoadIdentity();
    drawAxis();

    // set Camera position + orientation
    glLoadIdentity();
    m_Camera.render();

    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    // bind world texture
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texture);

    // create world sphere
    GLUquadricObj *gSphere = gluNewQuadric();
    gluQuadricTexture(gSphere, GL_TRUE);
    gluQuadricNormals(gSphere, GLU_SMOOTH);
    gluSphere(gSphere, 10, 50, 50);

    gluDeleteQuadric(gSphere);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    // draw Datapoints
    glColor3f(0.0, 0.0, 0.8);
    foreach(cFileData* file, drawList){
        file->drawPoints(10);
    }
//    foreach (cDrawObject<float>* obj, drawList){
//        obj->drawObject();
//    }

    glFlush();
}

void render_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width/(float)height;
    gluPerspective(60.0, aspect, 0.2, 60.0);

    glMatrixMode(GL_MODELVIEW);
}

void render_widget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void render_widget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    // rotate camera (left mouse button)
    if (event->buttons() & Qt::LeftButton) {
        m_Camera.rotateX(0.2*dy);
        m_Camera.rotateY(0.2*dx);
    }
    // move up-down left-right (right mouse button)
    else if (event->buttons() & Qt::RightButton) {
        m_Camera.moveRight(-0.05*dx);
        m_Camera.moveUpward(0.1*dy);
    }

    lastPos = event->pos();

    update();
}

void render_widget::wheelEvent(QWheelEvent *event){
    // mouse wheel control
    m_Camera.moveForward(0.01*event->delta());
    update();
}

void render_widget::keyPressEvent(QKeyEvent *event){

    int theKey = event->key();
    // execute key events
    switch (theKey){
    case Qt::Key_W:
       m_Camera.moveForward(0.2);
       break;
    case Qt::Key_S:
       m_Camera.moveForward(-0.2);
       break;
    case Qt::Key_A:
        m_Camera.moveRight(-0.2);
        break;
    case Qt::Key_D:
        m_Camera.moveRight(0.2);
        break;
    case Qt::Key_I:
        m_Camera.rotateX(1);
        break;
    case Qt::Key_K:
        m_Camera.rotateX(-1);
        break;
    case Qt::Key_J:
        m_Camera.rotateY(1);
        break;
    case Qt::Key_L:
        m_Camera.rotateY(-1);
        break;
    case Qt::Key_R:
        m_Camera.reset();
        break;
    case Qt::Key_F11:
        if (isFullScreen()){
            showNormal();
            update();
        }
        else{
            showFullScreen();
            update();
        }
        break;
    default:
        break;
    }

    update();
}
