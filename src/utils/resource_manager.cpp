//
// Created by fangl on 2023/9/22.
//

#include "utils/resource_manager.hpp"


// Global variables to store Shaders and Textures
std::map<QString, std::shared_ptr<Shader>> ResourceManager::map_Shaders;
std::map<QString, std::shared_ptr<Texture2D>> ResourceManager::map_Textures;

void ResourceManager::updateProjViewViewPosMatrixInShader(QMatrix4x4 proj, QMatrix4x4 vi, QVector3D viewP) {
    for(const auto& sha : map_Shaders) {
        sha.second->use();
        sha.second->setMatrix4f("projection", proj);
        sha.second->setMatrix4f("view", vi);
        sha.second->setVector3f("viewPos", viewP);
        sha.second->release();
    }
}

void ResourceManager::updateRenderConfigure(GLboolean depthMode) {
    for(const auto& sha : map_Shaders) {
        sha.second->use();
        sha.second->setBool("enableDepthMode", depthMode);
        sha.second->release();
    }
}

// 在GameObject里执行
void ResourceManager::updateMaterialInShader(const QString& sName, const Material& mat) {
    if(map_Shaders.find(sName) == map_Shaders.end()) {
        qFatal("Set Material Failed! No Target Shader Exists");
    }

    const auto& tempShader = map_Shaders[sName];
    tempShader->use();

    // -1 代表没有该textures
    if(mat.texture_diffuse1 != nullptr) {
        tempShader->setInteger("material.texture_diffuse1", mat.texture_diffuse1->getTextureID());
        tempShader->setBool("useDiffuseTexture", true);
    } else {
        tempShader->setBool("useDiffuseTexture", false);
    }

    if(mat.texture_specular1 != nullptr) {
        tempShader->setInteger("material.texture_specular1", mat.texture_specular1->getTextureID());
        tempShader->setBool("useSpecularTexture", true);
    } else {
        tempShader->setBool("useSpecularTexture", false);
    }

    tempShader->setFloat("material.shininess" ,mat.shininess);
    tempShader->setVector3f("material.ambientColor", mat.ambientColor);
    tempShader->setVector3f("material.diffuseColor", mat.diffuseColor);
    tempShader->setVector3f("material.specularColor", mat.specularColor);
    tempShader->setFloat("material.ambientOcclusion", mat.ambientOcclusion);

    tempShader->release();
}

void ResourceManager::updateDirectLightInShader(GLboolean enableLighting,DirectLight dl) {
    for(const auto& sha : map_Shaders) {
        sha.second->use();
        sha.second->setBool("useLight", enableLighting);
        sha.second->setVector3f("directLight.direction", dl.direction);
        sha.second->setVector3f("directLight.ambientColor", dl.ambientColor);
        sha.second->setVector3f("directLight.diffuseColor", dl.diffuseColor);
        sha.second->setVector3f("directLight.specularColor", dl.specularColor);
        sha.second->setFloat("directLight.intensity", dl.intensity);
        sha.second->release();
    }
}

void ResourceManager::updatePointLightInShader(PointLight pl) {
    for(const auto& sha : map_Shaders) {
        sha.second->use();
        sha.second->setVector3f("pointLight.position", pl.position);
        sha.second->setVector3f("pointLight.ambientColor", pl.ambientColor);
        sha.second->setVector3f("pointLight.diffuseColor", pl.diffuseColor);
        sha.second->setVector3f("pointLight.specularColor", pl.specularColor);
        sha.second->setFloat("pointLight.intensity", pl.intensity);
        sha.second->release();
    }
}

void ResourceManager::updateSpotLightInShader(SpotLight sl) {
    for(const auto& sha : map_Shaders) {
        sha.second->use();
        sha.second->setVector3f("spotLight.position", sl.position);
        sha.second->setVector3f("spotLight.direction", sl.direction);
        sha.second->setVector3f("spotLight.ambientColor", sl.ambientColor);
        sha.second->setVector3f("spotLight.diffuseColor", sl.diffuseColor);
        sha.second->setVector3f("spotLight.specularColor", sl.specularColor);
        sha.second->setFloat("spotLight.intensity", sl.intensity);
        sha.second->release();
    }
}

std::shared_ptr<Shader> ResourceManager::loadShader(const QString& name,
                                          const QString& vShaderFile,
                                          const QString& fShaderFile,
                                          const QString& gShaderfile) {
    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    bool isCompiledSuccess = shader->compile(vShaderFile,
                          fShaderFile,
                          gShaderfile == nullptr ? nullptr : gShaderfile);

    if(isCompiledSuccess) {
        qDebug() << "Successfully Loaded Shader : " << name;
        map_Shaders[name] = shader;
        return map_Shaders[name];
    } else {
        qDebug() << "Fail Loaded Shader : " << name;
        qFatal("WRONG SHADER LOADED!");
    }
}

std::shared_ptr<Shader> ResourceManager::getShader(const QString& name){
    if(map_Shaders.find(name) != map_Shaders.end())
        return map_Shaders[name];
    else {
        QString temp("No Shader : " + name + " Exist!");
        qFatal(temp.toStdString().c_str());
    }
}

std::shared_ptr<Texture2D> ResourceManager::loadTexture(const QString& name, const QString& file, GLboolean alpha){
    std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();

    if(alpha){
        texture->internal_format = QOpenGLTexture::RGBAFormat;
        texture->wrap_s = QOpenGLTexture::ClampToBorder;
        texture->wrap_t = QOpenGLTexture::ClampToBorder;
    }

    texture->generate(file);
    map_Textures[name] = texture;

    return texture;
}

std::shared_ptr<Texture2D> ResourceManager::getTexture(const QString& name){
    return map_Textures[name];
}

void ResourceManager::clearShader(){
    map_Shaders.clear();
}

void ResourceManager::clearTextures() {
    map_Textures.clear();
}

QVector<std::shared_ptr<Mesh>> ResourceManager::loadModel(const QString& mPath) {
    QVector<std::shared_ptr<Mesh>> meshes;

    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(mPath.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || !scene->mRootNode) {
        qCritical() << "ERROR::ASSIMP::" << import.GetErrorString() << Qt::endl;
    } else if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        qDebug() << "WARNING::ASSIMP::" << "Scene Flags Incomplete";
    }

    QString modelDirectory = mPath.left(mPath.lastIndexOf('/'));
    qDebug() << "Model Directory: " + modelDirectory;

    if(scene->mRootNode)
        meshes.append(processNode(scene->mRootNode, scene, modelDirectory));
    else
        qCritical("ERROR::ASSIMP::Scene Root Node is null.");

    return meshes;
}

QVector<std::shared_ptr<Mesh>> ResourceManager::processNode(aiNode *node, const aiScene *scene, const QString& mDir) {
    QVector<std::shared_ptr<Mesh>> meshes;

    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, mDir));
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        auto childMeshes = processNode(node->mChildren[i], scene, mDir);
        meshes.append(childMeshes);
    }

    return meshes;
}

std::shared_ptr<Mesh> ResourceManager::processMesh(aiMesh *mesh, const aiScene *scene, const QString& mDir) {
    bool haveNormal = false;
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<std::shared_ptr<Texture2D>> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        QVector3D vector;
        Vertex vertex;
        // 处理顶点位置、法线和纹理坐标
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.position = vector;

        // 处理索引
        if(mesh->mNormals != nullptr) {
            haveNormal = true;
            vector.setX(mesh->mNormals[i].x);
            vector.setY(mesh->mNormals[i].y);
            vector.setZ(mesh->mNormals[i].z);
            vertex.normal = vector;
        }

        if(mesh->mTextureCoords[0]) {
            QVector2D vec;
            vec.setX(mesh->mTextureCoords[0][i].x);
            vec.setY(mesh->mTextureCoords[0][i].y);
            vertex.texCoord = vec;
        } else {
            vertex.texCoord = QVector2D(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // if we have no normal there
    if(mesh->mNormals == nullptr) {
        reCalculateNormal(vertices, indices);
    }

    // 处理材质
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        QVector<std::shared_ptr<Texture2D>> diffuseMaps = loadMaterialTextures(material,
                                                                               aiTextureType_DIFFUSE,
                                                                               "texture_diffuse", mDir);
        textures.append(diffuseMaps);
        QVector<std::shared_ptr<Texture2D>> specularMaps = loadMaterialTextures(material,
                                                                                aiTextureType_SPECULAR,
                                                                                "texture_specular", mDir);
        textures.append(specularMaps);
    } else {
        qDebug() << "Current Model Has No Texture";
    }

    return std::make_shared<Mesh>(nullptr, vertices, indices, textures);
}

QVector<std::shared_ptr<Texture2D>> ResourceManager::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const QString& typeName, const QString& modelDirectory) {
    QVector<std::shared_ptr<Texture2D>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        QString qStr = modelDirectory + "/" + QString::fromUtf8(str.C_Str());
        qDebug() << "Load " << typeName << " : " << qStr;

        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
        texture->generate(qStr);
        texture->type = stringToTextureType(typeName);
        texture->path = qStr;

        textures.push_back(texture);
    }
    return textures;
}

void ResourceManager::reCalculateNormal(QVector<Vertex> &vertices, const QVector<unsigned int>& indices) {
    for(auto &v : vertices) {
        v.normal = QVector3D(0.0f,0.0f,0.0f);
    }

    for(int i = 0; i < indices.size(); i += 3) {
        int index1 = indices[i];
        int index2 = indices[i+1];
        int index3 = indices[i+2];

        QVector3D p1 = vertices[index1].position;
        QVector3D p2 = vertices[index2].position;
        QVector3D p3 = vertices[index3].position;

        QVector3D edge1 = p2 - p1;
        QVector3D edge2 = p3 - p1;

        QVector3D faceNormal = QVector3D::crossProduct(edge1, edge2);

        vertices[index1].normal += faceNormal;
        vertices[index1].normal += faceNormal;
        vertices[index1].normal += faceNormal;
    }

    for(auto &v : vertices) {
        v.normal.normalize();
    }
}
