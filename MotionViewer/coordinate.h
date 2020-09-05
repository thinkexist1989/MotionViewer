#ifndef COORDINATE_H
#define COORDINATE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Coordinate : protected QOpenGLFunctions
{
public:
    Coordinate();
    void init();
    void draw(QMatrix4x4 view, QMatrix4x4 projection, QMatrix4x4 model);

    void initAxis();
    void drawAxis();
    void initGrid();
    void drawGrid();

    void initVertexArray(float* vertices, unsigned long long len, QOpenGLVertexArrayObject* vao, QOpenGLBuffer* vbo);
private:
    QOpenGLShaderProgram sp; //coor.vert coor.frag
    QOpenGLVertexArrayObject* vaoX;
    QOpenGLBuffer* vboX;

    QOpenGLVertexArrayObject* vaoY;
    QOpenGLBuffer* vboY;

    QOpenGLVertexArrayObject* vaoZ;
    QOpenGLBuffer* vboZ;

    //for grid
    QOpenGLVertexArrayObject* vaoXZ;
    QOpenGLBuffer* vboXZ;

    QOpenGLVertexArrayObject* vaoYZ;
    QOpenGLBuffer* vboYZ;

    QOpenGLVertexArrayObject* vaoXY;
    QOpenGLBuffer* vboXY;
};

#endif // COORDINATE_H
