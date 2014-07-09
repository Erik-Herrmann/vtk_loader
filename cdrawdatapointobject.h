#ifndef CDRAWDATAPOINTOBJECT_H
#define CDRAWDATAPOINTOBJECT_H

#include "cadrawobject.h"
#include <QtOpenGL>
#include <QOpenGLVertexArrayObject>

template <typename T>
class cDrawDataPointObject : public caDrawObject
{
public:
    cDrawDataPointObject(QGLFunctions *func);
    cDrawDataPointObject(QGLFunctions *func, T *vertex, size_t size, GLenum type=GL_QUADS);
    virtual ~cDrawDataPointObject();

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
    GLuint m_texture;
    QGLBuffer *m_TextureCoordBuffer;
    QOpenGLVertexArrayObject *m_vao;

    QOpenGLShaderProgram *m_shaderProgram;

    T *m_VertexPtr;
    unsigned int m_size;

    void createVBO();
    void loadShaders();
    void generateQuads(T *points, T *quads, float *texCoord);

};

#include "cdrawdatapointobject.tcc"

#endif // CDRAWDATAPOINTOBJECT_H
