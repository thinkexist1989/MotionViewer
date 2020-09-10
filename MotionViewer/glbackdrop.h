#ifndef BACKDROP_H
#define BACKDROP_H

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class GLBackdrop : protected QOpenGLFunctions
{
public:
    GLBackdrop();
    void draw();
private:
    QOpenGLShaderProgram shader;
    QOpenGLBuffer vertices;
};

#endif // BACKDROP_H
