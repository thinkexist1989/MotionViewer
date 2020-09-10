#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glmesh.h>

#include <string>
#include <vector>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class GLModel: protected QOpenGLFunctions
{
public:
    // model data
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<GLMesh>    meshes;
    std::string directory;
    bool gammaCorrection;

    GLModel(std::string const &path, bool gamma = false);
    void Draw(QOpenGLShaderProgram* psp = nullptr);
    void draw(QMatrix4x4 view, QMatrix4x4 projection,QMatrix4x4 model);

    void setShader(const QString& vertex, const QString& fragment);
    void setColor(const QVector4D color);

    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

private:
    void loadModel(const std::string &path);

    void processNode(aiNode *node, const aiScene* scene);
    GLMesh processMesh(aiMesh *mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    QOpenGLShaderProgram sp;
    QMatrix4x4 model; // position and orietation of model
    QVector4D color = QVector4D(1.0f, 0.75f, 0.0f, 1.0f);

};

#endif // MODEL_H
