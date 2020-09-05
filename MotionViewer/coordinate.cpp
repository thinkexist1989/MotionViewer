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
    initAxis();
    initGrid();
}

void Coordinate::draw(QMatrix4x4 view, QMatrix4x4 projection, QMatrix4x4 model)
{
    sp.bind();

    sp.setUniformValue("view", view);
    sp.setUniformValue("projection", projection);
    sp.setUniformValue("model", model);

    drawAxis();

//    drawGrid();

    sp.release();
}

void Coordinate::initAxis()
{

    float verticesX[] = {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                         0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

//    initVertexArray(verticesX,sizeof (verticesX), vaoX, vboX);

    vaoX = new QOpenGLVertexArrayObject();
    if(vaoX->create())
        vaoX->bind();
    vboX = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboX->create();
    vboX->bind();
    vboX->allocate(verticesX, sizeof (verticesX));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboX->release();
    vaoX->release();


    float verticesY[] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                         0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};

//    initVertexArray(verticesY, sizeof (verticesY), vaoY, vboY);

    vaoY = new QOpenGLVertexArrayObject();
    if(vaoY->create())
        vaoY->bind();
    vboY = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboY->create();
    vboY->bind();
    vboY->allocate(verticesY, sizeof (verticesY));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboY->release();
    vaoY->release();


    float verticesZ[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                         0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 1.0f, 1.0f};

//    initVertexArray(verticesZ,sizeof (verticesZ), vaoZ, vboZ);

    vaoZ = new QOpenGLVertexArrayObject();
    if(vaoZ->create())
        vaoZ->bind();
    vboZ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboZ->create();
    vboZ->bind();
    vboZ->allocate(verticesZ, sizeof (verticesZ));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboZ->release();
    vaoZ->release();
}

void Coordinate::drawAxis()
{
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
}

void Coordinate::initGrid()
{
    float verticesXZ[] = {-5.0f, 0.0f, -5.0f, 1.0f, 0.0f, 1.0f, 0.1f,
                          -5.0f, 0.0f,  5.0f, 1.0f, 0.0f, 1.0f, 0.1f,
                           5.0f, 0.0f,  5.0f, 1.0f, 0.0f, 1.0f, 0.1f,
                           5.0f, 0.0f, -5.0f, 1.0f, 0.0f, 1.0f, 0.1f};

    vaoXZ = new QOpenGLVertexArrayObject();
    if(vaoXZ->create())
        vaoXZ->bind();
    vboXZ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboXZ->create();
    vboXZ->bind();
    vboXZ->allocate(verticesXZ, sizeof (verticesXZ));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboXZ->release();
    vaoXZ->release();

    float verticesYZ[] = { 0.0f, -5.0f, -5.0f, 0.0f, 1.0f, 1.0f, 0.1f,
                           0.0f, -5.0f,  5.0f, 0.0f, 1.0f, 1.0f, 0.1f,
                           0.0f,  5.0f,  5.0f, 0.0f, 1.0f, 1.0f, 0.1f,
                           0.0f,  5.0f, -5.0f, 0.0f, 1.0f, 1.0f, 0.1f};

    vaoYZ = new QOpenGLVertexArrayObject();
    if(vaoYZ->create())
        vaoYZ->bind();
    vboYZ = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboYZ->create();
    vboYZ->bind();
    vboYZ->allocate(verticesYZ, sizeof (verticesYZ));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboYZ->release();
    vaoYZ->release();

    float verticesXY[] = {-5.0f, -5.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.1f,
                          -5.0f, 5.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.1f,
                           5.0f, 5.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.1f,
                           5.0f, -5.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.1f};

    vaoXY = new QOpenGLVertexArrayObject();
    if(vaoXY->create())
        vaoXY->bind();
    vboXY = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboXY->create();
    vboXY->bind();
    vboXY->allocate(verticesXY, sizeof (verticesXY));
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vboXY->release();
    vaoXY->release();
}



void Coordinate::drawGrid()
{
    vaoXZ->bind();
//    glLineWidth(1.5f);
    glDrawArrays(GL_QUADS, 0, 4);
    vaoXZ->release();

    vaoXY->bind();
//    glLineWidth(1.5f);
    glDrawArrays(GL_QUADS, 0, 4);
    vaoXY->release();

    vaoYZ->bind();
//    glLineWidth(1.5f);
    glDrawArrays(GL_QUADS, 0, 4);
    vaoYZ->release();
}


//目前这个函数有点问题 sizeof(vertices)结果不对
void Coordinate::initVertexArray(float *vertices, unsigned long long len, QOpenGLVertexArrayObject *vao, QOpenGLBuffer *vbo)
{
    vao = new QOpenGLVertexArrayObject();
    if(vao->create())
        vao->bind();
    vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vbo->create();
    vbo->bind();
    vbo->allocate(vertices, len);
//    vboX->setUsagePattern(QOpenGLBuffer::StaticDraw);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

    vbo->release();
    vao->release();
}
