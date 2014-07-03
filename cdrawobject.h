#ifndef CDRAWOBJECT_H
#define CDRAWOBJECT_H

#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>

template <typename T>
class cDrawObject
{
public:
    cDrawObject(QGLFunctions *func);
    cDrawObject(QGLFunctions *func, T *vertex, size_t size, GLenum type=GL_POINTS);
    cDrawObject(QGLFunctions *func, T *vertex, GLubyte *color, size_t size, GLenum type=GL_POINTS);
    ~cDrawObject();

    void setVertices(T *vertices, size_t size);
    T* getVertices();
    void setColor(GLubyte *color);
    GLubyte* getColor();

    void setType(GLenum target);

    void drawObject();
    void drawIndexObject(std::vector<unsigned int> *indices);

private:
    QGLFunctions *m_glFunc;
    GLenum m_Type;

    QGLBuffer *m_VertexBuffer;
    QGLBuffer *m_ColorBuffer;
    QOpenGLVertexArrayObject *m_vao;

    QOpenGLShaderProgram *m_shaderProgram;

    T *m_VertexPtr;
    GLubyte *m_ColorPtr;
    unsigned int m_size;

    void createVBO();
    void loadShaders();

};

#include "cdrawobject.tcc"

#endif // CDRAWOBJECT_H
