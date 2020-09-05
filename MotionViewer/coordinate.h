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
    void draw(QMatrix4x4 view, QMatrix4x4 projection,QMatrix4x4 model);
private:
    QOpenGLShaderProgram sp; //coor.vert coor.frag
    QOpenGLVertexArrayObject* vaoX;
    QOpenGLBuffer* vboX;

    QOpenGLVertexArrayObject* vaoY;
    QOpenGLBuffer* vboY;

    QOpenGLVertexArrayObject* vaoZ;
    QOpenGLBuffer* vboZ;
};

#endif // COORDINATE_H
