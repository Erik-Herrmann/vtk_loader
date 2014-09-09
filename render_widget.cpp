#include "render_widget.h"

#include <QtGui>
#include <GL/glu.h>
#include <QPixmap>
#include <QImage>
#include <math.h>
#include "globaldefines.h"
#include "datafield/cdatafieldt.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif



render_widget::render_widget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SingleBuffer | QGL::DepthBuffer | QGL::StencilBuffer | QGL::Rgba | QGL::AlphaChannel), parent),
      m_WorldPatches(0)
{
    setWindowTitle("Render Widget");
    m_Camera.setPosition(QVector3D(0.0, 0.0, WORLD_SPHERE_RADIUS+10));
    texture = 0;
#if DRAWMODE_SPHERES_DISPLAYLIST
    disp = 0;
#endif
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

    unsigned int numTimeSlices = m_WorldPatches->setFileData(data);
    emit setSliderMinMax(0, numTimeSlices);

#if DRAWMODE_SPHERES
    drawList.push_back(data);
#endif
//----------------------------
#if (DRAWMODE_BUFFER || DRAWMODE_INDEXED_BUFFER)
    if (data->fileType() == MIPAS){
        std::vector<float> *pointData = new std::vector<float>;
        int size = data->getPointData(pointData);
        std::vector<unsigned char> *detect =
                static_cast<cDataFieldT<unsigned char>*>(data->getDatafield("detection"))->getDataVector();
        cDrawObjectQuadSpheres<float> *obj = new cDrawObjectQuadSpheres<float>(this->context()->functions(),
                                                         pointData->data(), detect->data(),
                                                         size, GL_QUADS);
        removeFromDrawlist(data);
        filedataDrawindexMap[data] = drawList.size();
            drawList.push_back(obj);

        delete pointData;
        delete detect;
    }
    else{
        std::vector<float> *pointData = new std::vector<float>;
        int size = data->getPointData(pointData);
        cDrawObject<float> *obj = new cDrawObject<float>(this->context()->functions(),
                                                         pointData->data(), size, GL_POINTS);
        removeFromDrawlist(data);
        filedataDrawindexMap[data] = drawList.size();
            drawList.push_back(obj);

        delete pointData;
    }
#endif
//----------------------------
#if DRAWMODE_SPHERES_DISPLAYLIST
    disp = data->getDisplayList();
#endif

    update();
}

void render_widget::removeFromDrawlist(cFileData *data){
#if DRAWMODE_SPHERES
    drawList.removeAll(data);
#endif
//------------------------
#if (DRAWMODE_BUFFER || DRAWMODE_INDEXED_BUFFER)
    if (filedataDrawindexMap.contains(data)){
        int id = filedataDrawindexMap[data];
        delete drawList.at(id);
        drawList.removeAt(id);
    }
#endif
//------------------------

    update();
}

void render_widget::loadTextures(){    
    QPixmap pixMap(QCoreApplication::applicationDirPath() + "/texture/earth_8k.jpg");
    if (pixMap.isNull())
        return;
    QImage *tex = new QImage(QGLWidget::convertToGLFormat(pixMap.toImage()));
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width(),
                 tex->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 tex->bits());

    delete tex;
}

void render_widget::initializeGL()
{
    makeCurrent();

    glClearColor (0.0, 0.0, 0.0, 1.0);

    // load world texture to GPU-Buffer
    loadTextures();

    m_WorldPatches = new cWorldPatchController();
    m_WorldPatches->createPatchs(PATCHES_LONGITUDE, PATCHES_LATITUDE);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glEnable(GL_TEXTURE_2D);


    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    static GLfloat light1Position[4] = { -0.5, -5.0, -7.0, -1.0 };
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
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

void drawVolcano(){
    // Grímsvötn (Island)
    glColor3f(1.0, 0.0, 0.0);
    GLUquadricObj *volc = gluNewQuadric();
    glPushMatrix();
    glTranslatef((WORLD_SPHERE_RADIUS*1.001)
                          *sin(THETA(VOLCANO_GRIMSVOTN_LATITUDE))
                          *sin(PHI(VOLCANO_GRIMSVOTN_LONGITUDE)),
                 (WORLD_SPHERE_RADIUS*1.001)
                          *cos(THETA(VOLCANO_GRIMSVOTN_LATITUDE)),
                 (WORLD_SPHERE_RADIUS*1.001)
                          *sin(THETA(VOLCANO_GRIMSVOTN_LATITUDE))
                          *cos(PHI(VOLCANO_GRIMSVOTN_LONGITUDE)));
    glRotatef(-VOLCANO_GRIMSVOTN_LATITUDE, 1.0, 0.0, 0.0);
    glRotatef(VOLCANO_GRIMSVOTN_LONGITUDE, 0.0, 1.0, 0.0);
    gluDisk(volc, 0,
            (0.005)*WORLD_SPHERE_RADIUS,
            10, 1);
    glPopMatrix();

    // Puyehue-Cordón Caulle (Chile)
    glPushMatrix();
    glTranslatef((WORLD_SPHERE_RADIUS*1.001)
                          *sin(THETA(VOLCANO_PUYEHUE_LATITUDE))
                          *sin(PHI(VOLCANO_PUYEHUE_LONGITUDE)),
                 (WORLD_SPHERE_RADIUS*1.001)
                          *cos(THETA(VOLCANO_PUYEHUE_LATITUDE)),
                 (WORLD_SPHERE_RADIUS*1.001)
                          *sin(THETA(VOLCANO_PUYEHUE_LATITUDE))
                          *cos(PHI(VOLCANO_PUYEHUE_LONGITUDE)));
    glRotatef(-VOLCANO_PUYEHUE_LATITUDE, 1.0, 0.0, 0.0);
    glRotatef(VOLCANO_PUYEHUE_LONGITUDE, 0.0, 1.0, 0.0);
    gluDisk(volc, 0,
            (0.005)*WORLD_SPHERE_RADIUS,
            10, 3);
    glPopMatrix();

    // Nabro (Eritrea)
    glPushMatrix();
    glTranslatef((WORLD_SPHERE_RADIUS*1.001)
                          *sin(THETA(VOLCANO_NABRO_LATITUDE))
                          *sin(PHI(VOLCANO_NABRO_LONGITUDE)),
                 (WORLD_SPHERE_RADIUS*1.001)
                          *cos(THETA(VOLCANO_NABRO_LATITUDE)),
                 (WORLD_SPHERE_RADIUS*1.001)
                          *sin(THETA(VOLCANO_NABRO_LATITUDE))
                          *cos(PHI(VOLCANO_NABRO_LONGITUDE)));
    glRotatef(-VOLCANO_NABRO_LATITUDE, 1.0, 0.0, 0.0);
    glRotatef(VOLCANO_NABRO_LONGITUDE, 0.0, 1.0, 0.0);
    gluDisk(volc, 0,
            (0.005)*WORLD_SPHERE_RADIUS,
            10, 3);
    glPopMatrix();
    gluDeleteQuadric(volc);
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
    glPushMatrix();
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        GLUquadricObj *gSphere = gluNewQuadric();
        gluQuadricTexture(gSphere, GL_TRUE);
        gluQuadricNormals(gSphere, GLU_SMOOTH);
        gluSphere(gSphere, WORLD_SPHERE_RADIUS, 50, 50);
        gluDeleteQuadric(gSphere);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    drawVolcano();

#if DRAWMODE_SPHERES_DISPLAYLIST
    glCallList(disp);
#endif

//     DRAW: with cFileData-function
//----------------------------------------
#if DRAWMODE_SPHERES
    foreach(cFileData* file, drawList){
        file->drawPoints(WORLD_SPHERE_RADIUS);
    }
#endif
//----------------------------------------


//     DRAW: with cDrawObject (glBuffer)
//----------------------------------------
#if DRAWMODE_BUFFER
    glLineWidth(3);
    glPointSize(4);
//    foreach (caDrawObject* obj, drawList){
//        obj->drawObject();
//    }
    glPointSize(1);
    glLineWidth(1);
#endif
//----------------------------------------

//    DRAW: with indexed array (+/-)
//----------------------------------------
#if DRAWMODE_INDEXED_BUFFER
    foreach (caDrawObject *obj, drawList){
        obj->drawIndexObject(&indices);
    }
#endif
//----------------------------------------

//     TESTING SPHERE COORDINATES
//--------------------------------------------------------
//    glLineWidth(2);
//    for (int k=0; k < 30; k+=10){
//        glBegin(GL_LINE_STRIP);
//        for (int i=-90; i < 90; ++i){
//            glVertex3f(11*sin(THETA(i))*sin(PHI(k)),
//                       11*cos(THETA(i)),
//                       11*sin(THETA(i))*cos(PHI(k)));
//        }
//        glEnd();
//    }
//    glLineWidth(1);
//--------------------------------------------------------

    glDisable(GL_LIGHTING);
    m_WorldPatches->drawPatchs();

    glFlush();
}

void render_widget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float aspect = (float)width/(float)height;
    gluPerspective(45.0, aspect, 0.2, 100.0);

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
//#if DRAWMODE_INDEXED_BUFFER
//    case Qt::Key_Plus:
//        for (int i=0; i < 13; i++)
//            indices.push_back(indices.size());
//        break;
//    case Qt::Key_Minus:
//        for (int i=0; i < 13; i++)
//            indices.pop_back();
//        break;
//#endif
    default:
        break;
    }

    update();
}


void render_widget::forwardedKeys(QKeyEvent *event){
    keyPressEvent(event);
}


void render_widget::sliderValChanged(int position){
    if (m_WorldPatches)
        if (m_WorldPatches->isFileDataSet())
            m_WorldPatches->updatePatches(&indices, (unsigned int)position);

    update();
}
