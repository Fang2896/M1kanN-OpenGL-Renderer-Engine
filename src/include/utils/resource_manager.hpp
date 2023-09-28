//
// Created by fangl on 2023/9/22.
//

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <QString>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "m_type.hpp"
#include "data_structures.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture2d.hpp"


class ResourceManager
{
   public:
    static std::map<QString, std::shared_ptr<Shader>> map_Shaders;
    static std::map<QString, std::shared_ptr<Texture2D>> map_Textures;

    static void updateProjViewMatrixInShader(QMatrix4x4 proj, QMatrix4x4 vi);
    static void updateViewPosInShader(QVector3D viewP);
    static void updateMaterialInShader(const QString& sName, const Material& mat);
    static void updateDirectLightInShader(DirectLight dl);
    static void updatePointLightInShader(PointLight pl);
    static void updateSpotLightInShader(SpotLight sl);

    static std::shared_ptr<Shader> loadShader(const QString& name,
                                              const QString& vShaderFile,
                                              const QString& fShaderFile,
                                              const QString& gShaderfile = nullptr);
    static std::shared_ptr<Shader> getShader(const QString&  name);
    static std::shared_ptr<Texture2D> loadTexture(const QString&  name, const QString& file, GLboolean alpha = false);
    static std::shared_ptr<Texture2D> getTexture(const QString&  name);

    static void clearShader();
    static void clearTextures();

    static QVector<std::shared_ptr<Mesh>> loadModel(const QString& mPath);

   private:
    ResourceManager() {}

   private:
    static QVector<std::shared_ptr<Mesh>> processNode(aiNode *node, const aiScene *scene, const QString& mDir);
    static std::shared_ptr<Mesh> processMesh(aiMesh *mesh, const aiScene *scene, const QString& mDir);
    static QVector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial *mat,
                                                                    aiTextureType type,
                                                                    const QString& typeName,
                                                                    const QString& mDir);

    static void reCalculateNormal(QVector<Vertex> &vertices, const QVector<unsigned int>& indices);

};


#endif  //RESOURCE_MANAGER_HPP
