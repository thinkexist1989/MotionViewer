#ifndef COORDINATE_H
#define COORDINATE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class GLCoordinate : protected QOpenGLFunctions
{
public:
    GLCoordinate();
    void init();
    void draw(QMatrix4x4 view, QMatrix4x4 projection, QMatrix4x4 model);

    void initAxis();
    void drawAxis();
    void initGrid();
    void drawGrid();

    void initVertexArray(float* gridVertices, unsigned long long len, QOpenGLVertexArrayObject* vao, QOpenGLBuffer* vbo);
private:
    QOpenGLShaderProgram sp; //coor.vert coor.frag
    QOpenGLVertexArrayObject* vaoX;
    QOpenGLBuffer* vboX;

    QOpenGLVertexArrayObject* vaoY;
    QOpenGLBuffer* vboY;

    QOpenGLVertexArrayObject* vaoZ;
    QOpenGLBuffer* vboZ;

    //for grid
    void genGridXYVertices(float width, float height, int stepWidth, int stepHeight, QVector4D color);

    QVector<float> gridVertices; // vertex buffer
    QVector<unsigned int> gridIndices; // Index buffer

    QOpenGLVertexArrayObject* vaoGrid;
    QOpenGLBuffer* vboGrid;
    QOpenGLBuffer* veoGrid;

    QOpenGLVertexArrayObject* vaoXZ;
    QOpenGLBuffer* vboXZ;

    QOpenGLVertexArrayObject* vaoYZ;
    QOpenGLBuffer* vboYZ;

    QOpenGLVertexArrayObject* vaoXY;
    QOpenGLBuffer* vboXY;
};

#endif // COORDINATE_H
