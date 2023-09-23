//
// Created by fangl on 2023/9/23.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include <QVector>
#include <QDebug>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "texture2d.hpp"


class Model {
   public:
    explicit Model(const char* path) { loadModel(path); }
    void draw(const Shader& shader);

   private:
    QVector<Mesh> meshes;
    QString directory;

    void loadModel(const QString &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    QVector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                         QString typeName);
};

#endif  //MODEL_HPP
