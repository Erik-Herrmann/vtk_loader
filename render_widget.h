#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QGLWidget>
#include <QList>
#include "datastruct.h"
#include "cfiledata.h"
#include "ccamera.h"
#include "cdrawobject.h"

#define checkImageWidth 64
#define checkImageHeight 64

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
    QColor qtGreen;
    QColor qtPurple;
    GLuint texture;

    GLubyte checkImage[checkImageHeight][checkImageWidth][4];
    QList<cFileData*> drawList;
//    QList<cDrawObject<float>*> drawList;
//    QMap<cFileData*, int> filedataDrawindexMap;

    cCamera m_Camera;
    QImage tex;

    void loadTextures();
};

#endif // RENDER_WIDGET_H
