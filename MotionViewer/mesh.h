#ifndef MESH_H
#define MESH_H

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <vector>
#include <string>

struct Vertex
{
    // position
    QVector3D Position;
    // normal
    QVector3D Normal;
    // texCoords
    QVector2D TexCoords;
    // tangent
    QVector3D Tangent;
    // bitangent
    QVector3D Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh: protected QOpenGLFunctions
{
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

    void Draw(QOpenGLShaderProgram* psp);

private:
    void setupMesh();

    QOpenGLVertexArrayObject *vao;
    QOpenGLBuffer *vbo;
    QOpenGLBuffer *veo;

};

#endif // MESH_H
