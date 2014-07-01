#include "cdrawobject.h"
#include "QtOpenGL"

template<typename T>
cDrawObject<T>::cDrawObject(QGLFunctions *func)
    : m_glFunc(func),
      m_Type(GL_POINTS), m_ID(0),
      m_Buffer(0), m_size(0), m_vao(new QOpenGLVertexArrayObject(this))
{
    m_vao->create();
}

template<typename T>
cDrawObject<T>::cDrawObject(QGLFunctions *func, T *data, size_t size, GLenum type)
    : m_glFunc(func),
      m_Type(type), m_ID(0),
      m_Buffer(data), m_size(size)
{
    createVBO();
}

template<typename T>
cDrawObject<T>::~cDrawObject(){
}

template<typename T>
void cDrawObject<T>::setBuffer(T *data, size_t size){
    m_Buffer = data;
    m_size = size;
    createVBO();
}

template<typename T>
T *cDrawObject<T>::getBuffer(){
    return m_Buffer;
}

template<typename T>
void cDrawObject<T>::setType(GLenum target){
    m_Type = target;
}

template<typename T>
void cDrawObject<T>::createVBO(){
    m_vao->bind();

    m_positions = new QGLBuffer;
    m_positions->create();
    m_positions->setUsagePattern(QGLBuffer::StreamDraw);
    m_positions->bind();
    m_positions->allocate(m_Buffer, m_size*sizeof(float));

//    m_glFunc->glGenBuffers(1, &m_ID);
//    m_glFunc->glBindBuffer(GL_ARRAY_BUFFER, m_ID);
//    m_glFunc->glBufferData(GL_ARRAY_BUFFER, m_size, 0, GL_STATIC_DRAW);
//    m_glFunc->glBindBuffer(GL_ARRAY_BUFFER, m_ID);
//    m_glFunc->glBufferData(GL_ARRAY_BUFFER, m_size, m_Buffer, GL_STATIC_DRAW);
}

template<typename T>
void cDrawObject<T>::drawObject(){
//    m_glFunc->glBindBuffer(GL_ARRAY_BUFFER, m_ID);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, 0);
//    glDrawArrays(m_Type, 0, m_size);
//    glDisableClientState(GL_VERTEX_ARRAY);
//    m_glFunc->glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_vao->bind();
    glDrawArrays(m_Type, 0, m_size);
}


