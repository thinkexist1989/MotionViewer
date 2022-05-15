#include "glbackdrop.h"

/**
 * @brief GLBackdrop::GLBackdrop
 * openGL相关 背景绘制
 */
GLBackdrop::GLBackdrop()
{
    initializeOpenGLFunctions();

    // 模型着色
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/quad.vert");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/quad.frag");
    shader.link();


    float vbuf[] = {
        -1, -1, 0.00, 0.10, 0.15,
        -1,  1, 0.03, 0.21, 0.26,
         1, -1, 0.00, 0.12, 0.18,
         1,  1, 0.06, 0.26, 0.30};

    vertices.create();
    vertices.bind();
    //根据大小申请内存
    vertices.allocate(vbuf, sizeof(vbuf));
    vertices.release();
}

void GLBackdrop::draw()
{
    shader.bind();
    vertices.bind();

    const GLuint vp = shader.attributeLocation("vertex_position");
    const GLuint vc = shader.attributeLocation("vertex_color");

    glEnableVertexAttribArray(vp);
    glEnableVertexAttribArray(vc);

    glVertexAttribPointer(vp, 2, GL_FLOAT, false,
                          5 * sizeof(GLfloat), 0);
    glVertexAttribPointer(vc, 3, GL_FLOAT, false,
                          5 * sizeof(GLfloat),
                          (GLvoid*)(2 * sizeof(GLfloat)));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

    vertices.release();
    shader.release();
}
