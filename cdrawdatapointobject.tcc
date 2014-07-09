#include "cdrawdatapointobject.h"
#include "QtOpenGL"
#include <QOpenGLShaderProgram>

template<typename T>
cDrawDataPointObject<T>::cDrawDataPointObject(QGLFunctions *func)
    : m_glFunc(func),
      m_Type(GL_QUADS), m_VertexBuffer(0),
      m_vao(0), m_shaderProgram(0),
      m_VertexPtr(0), m_size(0)
{
    m_vao = new QOpenGLVertexArrayObject();
    m_vao->create();
}

template<typename T>
cDrawDataPointObject<T>::cDrawDataPointObject(QGLFunctions *func, T *vertex, size_t size, GLenum type)
    : m_glFunc(func),
      m_Type(type), m_VertexBuffer(0),
      m_vao(0), m_shaderProgram(0),
      m_VertexPtr(vertex), m_size(size)
{
    m_vao = new QOpenGLVertexArrayObject();
    m_vao->create();
    createVBO();
}


template<typename T>
cDrawDataPointObject<T>::~cDrawDataPointObject(){
    m_VertexPtr = 0;

    m_TextureCoordBuffer->release();
    m_TextureCoordBuffer->destroy();
    m_VertexBuffer->release();
    m_VertexBuffer->destroy();

    m_vao->destroy();
    m_vao->deleteLater();
}

template<typename T>
void cDrawDataPointObject<T>::setVertices(T *vertices, size_t size){
    m_VertexPtr = vertices;
    m_size = size;
    createVBO();
}

template<typename T>
T *cDrawDataPointObject<T>::getVertices(){
    return m_VertexPtr;
}

template<typename T>
void cDrawDataPointObject<T>::setType(GLenum target){
    m_Type = target;
}

template<typename T>
void cDrawDataPointObject<T>::generateQuads(T *points, T *quads, float* texCoord){
    unsigned int q = 0;
    unsigned int t = 0;
    for (unsigned int i=0; i < m_size/3; ++i){
        // x-y-plane
        // p1
        quads[q++] = points[3*i+0]-0.01;
        quads[q++] = points[3*i+1]+0.01;
        quads[q++] = points[3*i+2];
        texCoord[t++] = 0.0;
        texCoord[t++] = 0.0;
        // p2
        quads[q++] = points[3*i+0]-0.01;
        quads[q++] = points[3*i+1]-0.01;
        quads[q++] = points[3*i+2];
        texCoord[t++] = 0.0;
        texCoord[t++] = 1.0;
        // p3
        quads[q++] = points[3*i+0]+0.01;
        quads[q++] = points[3*i+1]-0.01;
        quads[q++] = points[3*i+2];
        texCoord[t++] = 1.0;
        texCoord[t++] = 1.0;
        //p4
        quads[q++] = points[3*i+0]+0.01;
        quads[q++] = points[3*i+1]+0.01;
        quads[q++] = points[3*i+2];
        texCoord[t++] = 1.0;
        texCoord[t++] = 0.0;
        // x-z-plane
        // p1
        quads[q++] = points[3*i+0]-0.01;
        quads[q++] = points[3*i+1];
        quads[q++] = points[3*i+2]+0.01;
        texCoord[t++] = 0.0;
        texCoord[t++] = 0.0;
        // p2
        quads[q++] = points[3*i+0]-0.01;
        quads[q++] = points[3*i+1];
        quads[q++] = points[3*i+2]-0.01;
        texCoord[t++] = 0.0;
        texCoord[t++] = 1.0;
        // p3
        quads[q++] = points[3*i+0]+0.01;
        quads[q++] = points[3*i+1];
        quads[q++] = points[3*i+2]-0.01;
        texCoord[t++] = 1.0;
        texCoord[t++] = 1.0;
        // p4
        quads[q++] = points[3*i+0]+0.01;
        quads[q++] = points[3*i+1];
        quads[q++] = points[3*i+2]+0.01;
        texCoord[t++] = 1.0;
        texCoord[t++] = 0.0;

        // y-z-plane
        // p1
        quads[q++] = points[3*i+0];
        quads[q++] = points[3*i+1]+0.01;
        quads[q++] = points[3*i+2]-0.01;
        texCoord[t++] = 0.0;
        texCoord[t++] = 0.0;
        // p2
        quads[q++] = points[3*i+0];
        quads[q++] = points[3*i+1]-0.01;
        quads[q++] = points[3*i+2]-0.01;
        texCoord[t++] = 0.0;
        texCoord[t++] = 1.0;
        // p3
        quads[q++] = points[3*i+0];
        quads[q++] = points[3*i+1]-0.01;
        quads[q++] = points[3*i+2]+0.01;
        texCoord[t++] = 1.0;
        texCoord[t++] = 1.0;
        // p4
        quads[q++] = points[3*i+0];
        quads[q++] = points[3*i+1]+0.01;
        quads[q++] = points[3*i+2]+0.01;
        texCoord[t++] = 1.0;
        texCoord[t++] = 0.0;
    }
}

template<typename T>
void cDrawDataPointObject<T>::createVBO(){
    m_vao->bind();

    //loadShaders();
    float *texCoord = new float[12*(m_size-(m_size/3))];
    T *quads = new T[12*m_size];

    generateQuads(m_VertexPtr, quads, texCoord);

    m_VertexBuffer = new QGLBuffer;
    m_VertexBuffer->create();
    m_VertexBuffer->setUsagePattern(QGLBuffer::StaticDraw);
    m_VertexBuffer->bind();
    m_VertexBuffer->allocate(quads, 12*m_size*sizeof(T));
    glVertexPointer(3, GL_FLOAT, 0, 0);

    m_TextureCoordBuffer = new QGLBuffer;
    m_TextureCoordBuffer->create();
    m_TextureCoordBuffer->setUsagePattern(QGLBuffer::StaticDraw);
    m_TextureCoordBuffer->bind();
    m_TextureCoordBuffer->allocate(texCoord, 12*(m_size-(m_size/3))*sizeof(unsigned int));
    glTexCoordPointer(2, GL_FLOAT, 0, 0);



    QImage tex(QGLWidget::convertToGLFormat(
                   QImage(
                       QCoreApplication::applicationDirPath() +
                        QString("/texture/dot.png"))
                   )
               );


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(),
                 tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 tex.bits());

    delete texCoord;
    delete quads;

    m_vao->release();

}

template<typename T>
void cDrawDataPointObject<T>::loadShaders(){
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
void cDrawDataPointObject<T>::drawObject(){
    m_vao->bind();

    glDisable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_LIGHTING);
    glAlphaFunc(GL_GREATER,0.4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glDrawArrays(m_Type, 0, m_size/3);

    glDisable(GL_BLEND);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_CULL_FACE);

    m_vao->release();
}


template<typename T>
void cDrawDataPointObject<T>::drawIndexObject(std::vector<unsigned int> *indices){
    m_vao->bind();

    glDisable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_LIGHTING);
    glAlphaFunc(GL_GREATER,0.4);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glDrawElements(m_Type, indices->size(), GL_UNSIGNED_INT, indices->data());

    glDisable(GL_BLEND);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_ALPHA_TEST);
    glEnable(GL_CULL_FACE);

    m_vao->release();
}


