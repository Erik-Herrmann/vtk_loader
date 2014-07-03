#include "cdrawobject.h"
#include "QtOpenGL"
#include <QOpenGLShaderProgram>

template<typename T>
cDrawObject<T>::cDrawObject(QGLFunctions *func)
    : m_glFunc(func),
      m_Type(GL_POINTS), m_VertexBuffer(0),
      m_vao(0), m_shaderProgram(0),
      m_VertexPtr(0), m_ColorPtr(0), m_size(0)
{
    m_vao = new QOpenGLVertexArrayObject();
//    m_shaderProgram = new QOpenGLShaderProgram();
    m_vao->create();
}

template<typename T>
cDrawObject<T>::cDrawObject(QGLFunctions *func, T *vertex, size_t size, GLenum type)
    : m_glFunc(func),
      m_Type(type), m_VertexBuffer(0),
      m_vao(0), m_shaderProgram(0),
      m_VertexPtr(vertex), m_ColorPtr(0), m_size(size)
{
    m_vao = new QOpenGLVertexArrayObject();
//    m_shaderProgram = new QOpenGLShaderProgram();
    m_vao->create();
    createVBO();
}

template<typename T>
cDrawObject<T>::cDrawObject(QGLFunctions *func, T *vertex, GLubyte *color, size_t size, GLenum type)
    : m_glFunc(func),
      m_Type(type), m_VertexBuffer(0),
      m_vao(0), m_shaderProgram(0),
      m_VertexPtr(vertex), m_ColorPtr(color), m_size(size)
{
    m_vao = new QOpenGLVertexArrayObject();
//    m_shaderProgram = new QOpenGLShaderProgram();
    m_vao->create();
    createVBO();
}

template<typename T>
cDrawObject<T>::~cDrawObject(){
    m_VertexPtr = 0;
    m_ColorPtr = 0;
    m_VertexBuffer->release();
    m_VertexBuffer->destroy();

//    m_shaderProgram->removeAllShaders();
//    m_shaderProgram->deleteLater();

    m_vao->destroy();
    m_vao->deleteLater();
}

template<typename T>
void cDrawObject<T>::setVertices(T *vertices, size_t size){
    m_VertexPtr = vertices;
    m_size = size;
    createVBO();
}

template<typename T>
T *cDrawObject<T>::getVertices(){
    return m_VertexPtr;
}

template<typename T>
void cDrawObject<T>::setColor(GLubyte *color){
    m_ColorPtr = color;
}

template<typename T>
GLubyte *cDrawObject<T>::getColor(){
    return m_ColorPtr;
}

template<typename T>
void cDrawObject<T>::setType(GLenum target){
    m_Type = target;
}

template<typename T>
void cDrawObject<T>::createVBO(){
    m_vao->bind();

    //loadShaders();

    m_VertexBuffer = new QGLBuffer;
    m_VertexBuffer->create();
    m_VertexBuffer->setUsagePattern(QGLBuffer::StaticDraw);
    m_VertexBuffer->bind();
    m_VertexBuffer->allocate(m_VertexPtr, m_size*sizeof(float));
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if (m_ColorPtr){
        m_ColorBuffer = new QGLBuffer;
        m_ColorBuffer->create();
        m_ColorBuffer->setUsagePattern(QGLBuffer::StaticDraw);
        m_ColorBuffer->bind();
        m_ColorBuffer->allocate(m_ColorPtr, m_size*sizeof(GLubyte));
        glColorPointer(3, GL_UNSIGNED_BYTE, 0, 0);
    }

//    m_shaderProgram->link();
//    m_shaderProgram->bind();

    m_vao->release();

}

template<typename T>
void cDrawObject<T>::loadShaders(){
    bool result = true;
    result = m_shaderProgram->addShaderFromSourceFile(
                                    QOpenGLShader::Vertex,
                                    QCoreApplication::applicationDirPath() +
                                    "/shader/pointShader.vert");

    if (!result)
        printf("Couldn't load ShaderProgram: Vertex\n");

    result = m_shaderProgram->addShaderFromSourceFile(
                                    QOpenGLShader::Fragment,
                                    QCoreApplication::applicationDirPath() +
                                    "/shader/pointShader.frag");
    if (!result)
        printf("Couldn't load ShaderProgram: Fragment\n");
}

template<typename T>
void cDrawObject<T>::drawObject(){
    m_vao->bind();

//    int posAttrib = m_shaderProgram->attributeLocation("position");
//    m_shaderProgram->setAttributeBuffer(posAttrib, GL_FLOAT, 0, 3 );
//    m_shaderProgram->enableAttributeArray(posAttrib);

    glEnableClientState(GL_VERTEX_ARRAY);
    if (m_ColorPtr){
        glEnableClientState(GL_COLOR_ARRAY);
    }

    glDrawArrays(m_Type, 0, m_size/3);

    if (m_ColorPtr){
        glDisableClientState(GL_COLOR_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);

    m_vao->release();
}


template<typename T>
void cDrawObject<T>::drawIndexObject(std::vector<unsigned int> *indices){
    m_vao->bind();

    glEnableClientState(GL_VERTEX_ARRAY);
    if (m_ColorPtr){
        glEnableClientState(GL_COLOR_ARRAY);
    }

    glDrawElements(m_Type, indices->size(), GL_UNSIGNED_INT, indices->data());

    if (m_ColorPtr){
        glDisableClientState(GL_COLOR_ARRAY);
    }
    glDisableClientState(GL_VERTEX_ARRAY);

    m_vao->release();
}


