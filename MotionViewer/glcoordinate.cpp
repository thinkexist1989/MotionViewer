#include "glcoordinate.h"

GLCoordinate::GLCoordinate()
{
    initializeOpenGLFunctions(); //必须先调用次函数获取OpenGL上下文
    sp.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/coor.vert");
    sp.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/coor.frag");
    sp.link();
    init();
}

void GLCoordinate::init()
{
    initAxis();
    initGrid();
}

void GLCoordinate::draw(QMatrix4x4 view, QMatrix4x4 projection, QMatrix4x4 model)
{
    sp.bind();

    sp.setUniformValue("view", view);
    sp.setUniformValue("projection", projection);
    sp.setUniformValue("model", model);

    drawAxis();

//    drawGrid(); //xy

    model.rotate(90, QVector3D(0.0f, 1.0f, 0.0f));
    sp.setUniformValue("model", model);
    drawGrid(); //yz

//    model.rotate(90, QVector3D(1.0f, 0.0f, 0.0f));
//    sp.setUniformValue("model", model);
//    drawGrid(); //xz

    sp.release();
}

void GLCoordinate::initAxis()
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


    float verticesY[] = {0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                         0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};

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


    float verticesZ[] = {0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                         0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f};

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

void GLCoordinate::drawAxis()
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

void GLCoordinate::initGrid()
{
    genGridXYVertices(2.0f, 2.0f, 20, 20, QVector4D(0.3f, 0.3f, 0.3f, 0.5f));

    vaoGrid = new QOpenGLVertexArrayObject();
    if(vaoGrid->create())
        vaoGrid->bind();
    vboGrid = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vboGrid->create();
    vboGrid->bind();
    vboGrid->allocate(gridVertices.data(), sizeof(float)*gridVertices.size());

    veoGrid = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    veoGrid->create();
    veoGrid->bind();
    veoGrid->allocate(gridIndices.data(), sizeof(unsigned int)*gridIndices.size());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), reinterpret_cast<void*>(3* sizeof(float)));

//    veoGrid->release();
    vboGrid->release();
    vaoGrid->release();

}



void GLCoordinate::drawGrid()
{
    vaoGrid->bind();
    glLineWidth(1.5f);
//    glDrawArrays(GL_QUADS, 0, 4);
    glDrawElements(GL_LINES, veoGrid->size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    vaoGrid->release();
}


//目前这个函数有点问题 sizeof(vertices)结果不对
void GLCoordinate::initVertexArray(float *vertices, unsigned long long len, QOpenGLVertexArrayObject *vao, QOpenGLBuffer *vbo)
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

void GLCoordinate::genGridXYVertices(float width, float height, int stepWidth, int stepHeight, QVector4D color)
{
    float stripWidth = width / stepWidth; //width 步进距离
    float stripHeight = height / stepHeight; // height 步进距离

    gridVertices.clear();
    gridVertices << -width/2 << -height/2 << 0.0f << color.x() << color.y() << color.z() << color.w()
             <<  width/2 << -height/2 << 0.0f << color.x() << color.y() << color.z() << color.w()
             <<  width/2 <<  height/2 << 0.0f << color.x() << color.y() << color.z() << color.w()
             << -width/2 <<  height/2 << 0.0f << color.x() << color.y() << color.z() << color.w();

    gridIndices.clear();
    gridIndices << 0 << 1
                << 1 << 2
                << 2 << 3
                << 3 << 0;

    int st = gridVertices.size()/7;
    qDebug() << st;

    for(int i = 1; i < stepWidth; i++)
    {
        gridVertices << -width/2 + stripWidth*i << -height/2 << 0.0f << color.x() << color.y() << color.z() << color.w()
                    << -width/2 + stripWidth*i << height/2 << 0.0f << color.x() << color.y() << color.z() << color.w();

        gridIndices << st + 2*(i-1) << st + 1 + 2*(i-1);
    }

    st = gridVertices.size()/7;

    for(int j = 1; j < stepHeight; j++)
    {
        gridVertices << -width/2 << -height/2 + stripHeight*j << 0.0f << color.x() << color.y() << color.z() << color.w()
                     <<  width/2 << -height/2 + stripHeight*j << 0.0f << color.x() << color.y() << color.z() << color.w();

        gridIndices << st + 2*(j-1) << st +1 + 2*(j-1);
    }

//    qDebug() << gridVertices;
}
