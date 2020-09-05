#include "coordinate.h"

Coordinate::Coordinate()
{
    initializeOpenGLFunctions();
    sp.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/coor.vert");
    sp.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/coor.frag");
    sp.link();
    init();
}

void Coordinate::init()
{

    float verticesX[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                         0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f};

    vaoX = new QOpenGLVertexArrayObject();
    if(vaoX->create())
        vaoX->bind();
    vboX = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboX->create();
    vboX->bind();
    vboX->allocate(verticesX, sizeof (verticesX));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboX->release();
    vaoX->release();


    float verticesY[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f};

    vaoY = new QOpenGLVertexArrayObject();
    if(vaoY->create())
        vaoY->bind();
    vboY = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboY->create();
    vboY->bind();
    vboY->allocate(verticesY, sizeof (verticesY));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboY->release();
    vaoY->release();


    float verticesZ[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                         0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f};

    vaoZ = new QOpenGLVertexArrayObject();
    if(vaoZ->create())
        vaoZ->bind();
    vboZ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboZ->create();
    vboZ->bind();
    vboZ->allocate(verticesZ, sizeof (verticesZ));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboZ->release();
    vaoZ->release();

}

void Coordinate::draw(QMatrix4x4 view, QMatrix4x4 projection, QMatrix4x4 model)
{
    sp.bind();

    sp.setUniformValue("view", view);
    sp.setUniformValue("projection", projection);
    sp.setUniformValue("model", model);

    vaoX->bind();
    glLineWidth(1.5f);
    glDrawArrays(GL_LINES, 0, 2);
    vaoX->release();

    vaoY->bind();
    glLineWidth(1.5f);
    glDrawArrays(GL_LINES, 0, 2);
    vaoY->release();

    vaoZ->bind();
    glLineWidth(1.5f);
    glDrawArrays(GL_LINES, 0, 2);
    vaoZ->release();

    sp.release();
}
