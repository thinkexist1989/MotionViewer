#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>

#include <string>
#include <vector>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Model: protected QOpenGLFunctions
{
public:
    // model data
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    std::string directory;
    bool gammaCorrection;

    Model(std::string const &path, bool gamma = false);
    void Draw(QOpenGLShaderProgram* psp = nullptr);
    void draw(QMatrix4x4 view, QMatrix4x4 projection,QMatrix4x4 model);

    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

private:
    void loadModel(const std::string &path);

    void processNode(aiNode *node, const aiScene* scene);
    Mesh processMesh(aiMesh *mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    QOpenGLShaderProgram sp;
    QMatrix4x4 model; // position and orietation of model

};

#endif // MODEL_H
