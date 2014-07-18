#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QGLWidget>
#include <QList>
#include "datastruct.h"
#include "cfiledata.h"
#include "ccamera.h"
#include "cadrawobject.h"
#include "cdrawobject.h"
#include "cdrawobjectquadspheres.h"
#include "cworldpatchcontroller.h"

#define DRAWMODE_BUFFER 1
#define DRAWMODE_INDEXED_BUFFER 0
#define DRAWMODE_SPHERES 0
#define DRAWMODE_SPHERES_DISPLAYLIST 0


class render_widget : public QGLWidget
{
    Q_OBJECT

public:
    render_widget(QWidget *parent = 0);
    ~render_widget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

public slots:
    void addToDrawlist(cFileData* data);
    void removeFromDrawlist(cFileData* data);

    void sliderValChanged(int position);

signals:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int xRot;
    int yRot;
    int zRot;
    QPoint lastPos;
    GLuint texture;
#if DRAWMODE_SPHERES_DISPLAYLIST
    GLuint disp;
#endif
#if DRAWMODE_INDEXED_BUFFER
    std::vector<unsigned int> indices;
#endif
#if DRAWMODE_SPHERES
    QList<cFileData*> drawList;
#endif
//-----------------------
#if (DRAWMODE_BUFFER || DRAWMODE_INDEXED_BUFFER)
    QList<caDrawObject*> drawList;
    QMap<cFileData*, int> filedataDrawindexMap;
//-----------------------
#endif
    cCamera m_Camera;
    QImage *tex;

    cWorldPatchController* patch;


    void loadTextures();
};

#endif // RENDER_WIDGET_H
