#ifndef CDRAWOBJECT_H
#define CDRAWOBJECT_H

#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>

template <typename T>
class cDrawObject
{
public:
    cDrawObject(QGLFunctions *func);
    cDrawObject(QGLFunctions *func, T *data, size_t size, GLenum type=GL_POINTS);
    ~cDrawObject();

    void setBuffer(T *data, size_t size);
    T* getBuffer();

    void setType(GLenum target);

    void drawObject();

private:
    QGLFunctions *m_glFunc;
    GLenum m_Type;
    GLuint m_ID;

    QGLBuffer *m_positions;
    QOpenGLVertexArrayObject *m_vao;

    T *m_Buffer;
    unsigned int m_size;

    void createVBO();

};

#include "cdrawobject.tcc"

#endif // CDRAWOBJECT_H
