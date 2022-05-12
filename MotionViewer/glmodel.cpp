//#include "glmodel.h"

//#include <iostream>
//#include <stb_image.h>
//#include <QResource>
//#include <QString>

///**
// * @brief GLModel::GLModel
// * @param path
// * @param gamma
// * opengl里的模型绘制
// */

//GLModel::GLModel(const std::string &path, bool gamma) : gammaCorrection(gamma)
//{
//    initializeOpenGLFunctions();
//    loadModel(path); // load model file

//    /* model_loading shader 模型着色 */
//    sp.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/gl/model_loading.vert");
//    sp.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/gl/model_loading.frag");
//    sp.link();
//}

//void GLModel::Draw(QOpenGLShaderProgram *psp)
//{
//    sp.bind();
////    std::cout <<"meshes size is: " <<meshes.size() << std::endl;
//    for(unsigned int i = 0; i < meshes.size(); i++)
//    {
//        if(psp)
//            meshes[i].Draw(psp);
//        else
//            meshes[i].Draw(&sp);
//    }
//    sp.release();
//}

//void GLModel::draw(QMatrix4x4 view, QMatrix4x4 projection, QMatrix4x4 model)
//{
//    sp.bind();
//    sp.setUniformValue("view", view);
//    sp.setUniformValue("projection", projection);
//    sp.setUniformValue("model", model);
//    sp.setUniformValue("color", color);

//    for(unsigned int i = 0; i < meshes.size(); i++)
//    {
//        meshes[i].Draw(&sp);
//    }
//    sp.release();
//}

//void GLModel::setShader(const QString &vertex, const QString &fragment)
//{
//    sp.removeAllShaders();
//    sp.addShaderFromSourceFile(QOpenGLShader::Vertex, vertex);
//    sp.addShaderFromSourceFile(QOpenGLShader::Fragment, fragment);
//    sp.link();
//}

//void GLModel::setColor(const QVector4D color)
//{
//    this->color = color;
//}

///**
// * @brief GLModel::loadModel
// * @param path
// * 加载模型文件
// */
//void GLModel::loadModel(const std::string &path)
//{
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);

//    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
//    {
//        std::cout << "ERROR::ASSIMP:: " <<importer.GetErrorString() << std::endl;
//        return;
//    }

//    directory = path.substr(0, path.find_last_of('/'));
//    std::cout << "model directory is: " << directory << std::endl;

//    processNode(scene->mRootNode, scene);
////    std::cout << "load model OK!" << std::endl;
//}

//void GLModel::processNode(aiNode *node, const aiScene *scene)
//{
//    for(unsigned int i = 0; i < node->mNumMeshes; i++)
//    {
//        // the node object only contains indices to index the actual objects in the scene.
//        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(processMesh(mesh, scene));
//    }
//    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
//    for(unsigned int i = 0; i < node->mNumChildren; i++)
//    {
//        processNode(node->mChildren[i], scene);
//    }
//}

//GLMesh GLModel::processMesh(aiMesh *mesh, const aiScene *scene)
//{
//    std::cout << "Processing Meshes!" << std::endl;
//    // data to fill
//    std::vector<Vertex> vertices;
//    std::vector<unsigned int> indices;
//    std::vector<Texture> textures;

////    std::cout <<"processMesh::mesh::mNumVertices " << mesh->mNumVertices << std::endl;
//    // walk through each of the mesh's vertices
//    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
//    {
//        Vertex vertex;
//        QVector3D vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
//        // positions
//        vector.setX(mesh->mVertices[i].x);
//        vector.setY(mesh->mVertices[i].y);
//        vector.setZ(mesh->mVertices[i].z);
//        vertex.Position = vector;
//        // normals
//        vector.setX(mesh->mNormals[i].x);
//        vector.setY(mesh->mNormals[i].y);
//        vector.setZ(mesh->mNormals[i].z);
//        vertex.Normal = vector;
//        // texture coordinates
//        if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
//        {
////            std::cout <<"processMesh::mesh::mTextureCoords " << mesh->mTextureCoords[0] << std::endl;
//            QVector2D vec;
//            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
//            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
//            vec.setX(mesh->mTextureCoords[0][i].x);
//            vec.setY(mesh->mTextureCoords[0][i].y);
//            vertex.TexCoords = vec;
//        }
//        else
//        {
////            std::cout <<"processMesh::mesh::mTextureCoords::else" << std::endl;
//            vertex.TexCoords = QVector2D(0.0f, 0.0f);
//        }
////        // tangent
////        vector.setX(mesh->mTangents[i].x);
////        vector.setY(mesh->mTangents[i].y);
////        vector.setZ(mesh->mTangents[i].z);
////        vertex.Tangent = vector;
////        // bitangent
////        vector.setX(mesh->mBitangents[i].x);
////        vector.setY(mesh->mBitangents[i].y);
////        vector.setZ(mesh->mBitangents[i].z);
////        vertex.Bitangent = vector;
//        vertices.push_back(vertex);
//    }
//    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
//    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
//    {
//        aiFace face = mesh->mFaces[i];
//        // retrieve all indices of the face and store them in the indices vector
//        for(unsigned int j = 0; j < face.mNumIndices; j++)
//            indices.push_back(face.mIndices[j]);
//    }
//    // process materials
//    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
//    // Same applies to other texture as the following list summarizes:
//    // diffuse: texture_diffuseN
//    // specular: texture_specularN
//    // normal: texture_normalN

//    // 1. diffuse maps
//    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//    // 2. specular maps
//    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//    // 3. normal maps
//    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
//    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
//    // 4. height maps
//    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
//    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

//    // return a mesh object created from the extracted mesh data
//    //            return Mesh(vertices, indices, textures);
//    std::cout << "Process Meshes OK!" << std::endl;
//    return GLMesh(vertices, indices, textures);
//}
///**
// * @brief GLModel::loadMaterialTextures
// * @param mat
// * @param type
// * @param typeName
// * @return
// * 加载材质纹理
// */
//std::vector<Texture> GLModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
//{
//    std::vector<Texture> textures;
//    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//    {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        std::cout << "Get Texture: " << str.C_Str() << std::endl;
//        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//        bool skip = false;
//        for(unsigned int j = 0; j < textures_loaded.size(); j++)
//        {
//            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
//            {
//                textures.push_back(textures_loaded[j]);
//                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//                break;
//            }
//        }
//        if(!skip)
//        {   // if texture hasn't been loaded already, load it
//            Texture texture;
//            texture.id = TextureFromFile(str.C_Str(), this->directory);
//            texture.type = typeName;
//            texture.path = str.C_Str();
//            textures.push_back(texture);
//            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
//        }
//    }
//    return textures;
//}


//unsigned int GLModel::TextureFromFile(const char *path, const std::string &directory, bool gamma)
//{
//    std::string filename = std::string(path);
//    filename = directory + '/' + filename;

//    unsigned int textureID;
//    glGenTextures(1, &textureID);

//    int width, height, nrComponents;
//    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
//    if (data)
//    {
//        GLenum format;
//        if (nrComponents == 1)
//            format = GL_RED;
//        else if (nrComponents == 3)
//            format = GL_RGB;
//        else if (nrComponents == 4)
//            format = GL_RGBA;

//        glBindTexture(GL_TEXTURE_2D, textureID);
//        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

//        glGenerateMipmap(GL_TEXTURE_2D);


//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//        stbi_image_free(data);
//    }
//    else
//    {
//        std::cout << "Texture failed to load at path: " << path << std::endl;
//        stbi_image_free(data);
//    }

//    std::cout << "texture filename is: " << filename << std::endl;
//    return textureID;
//}
