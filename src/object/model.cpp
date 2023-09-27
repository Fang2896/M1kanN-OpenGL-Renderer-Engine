//
// Created by fangl on 2023/9/23.
//


//#include "object/model.hpp"


//Model::Model(const char* mPath)
//    : Object("model", "defaultModel", "defaultModelShader"), modelPath(mPath) {}
//
//Model::Model(QString objName, const char* mPath)
//    : Object("model", std::move(objName), "defaultModelShader"), modelPath(mPath) {}
//
//void Model::init(QString shaderName) {
//    this->setShaderName(shaderName);
//    if(modelPath.isEmpty()) {
//        qFatal("Model Path Empty!");
//    }
//
//    loadModel(modelPath);
//    qDebug() << "======= Done Init Test Model ========";
//}
//
//void Model::init() {
//    this->setShaderName("defaultModelShader");
//    loadModel(modelPath);
//    qDebug() << "======= Done Init Model :" << getObjectName() << " Shader: "
//        << getShaderName() << " ========";
//}
//
//void Model::draw() {
//    for(auto & mesh : meshes)
//        mesh.draw(getShaderName());
//}
//
//void Model::loadModel(const QString& path) {
//    Assimp::Importer import;
//    const aiScene *scene = import.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcess_FlipUVs);
//
//    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        qDebug() << "ERROR::ASSIMP::" << import.GetErrorString() << Qt::endl;
//        return;
//    }
//
//    modelDirectory = path.left(path.lastIndexOf('/'));
//    qDebug() << "Model Directory: " + modelDirectory;
//    if(scene->mRootNode)
//      processNode(scene->mRootNode, scene);
//    else
//      qFatal("ERROR::ASSIMP::Scene Root Node is null.");
//}
//
//void Model::processNode(aiNode *node, const aiScene *scene) {
//    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
//        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(processMesh(mesh, scene));
//    }
//
//    for(unsigned int i = 0; i < node->mNumChildren; i++) {
//        processNode(node->mChildren[i], scene);
//    }
//}
//
//Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
//    QVector<Vertex> vertices;
//    QVector<unsigned int> indices;
//    QVector<std::shared_ptr<Texture2D>> textures;
//
//    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
//        QVector3D vector;
//        Vertex vertex;
//        // 处理顶点位置、法线和纹理坐标
//        vector.setX(mesh->mVertices[i].x);
//        vector.setY(mesh->mVertices[i].y);
//        vector.setZ(mesh->mVertices[i].z);
//        vertex.position = vector;
//
//        // 处理索引
//        vector.setX(mesh->mNormals[i].x);
//        vector.setY(mesh->mNormals[i].y);
//        vector.setZ(mesh->mNormals[i].z);
//        vertex.normal = vector;
//
//        if(mesh->mTextureCoords[0]) {
//            QVector2D vec;
//            vec.setX(mesh->mTextureCoords[0][i].x);
//            vec.setY(mesh->mTextureCoords[0][i].y);
//            vertex.texCoord = vec;
//        } else {
//            vertex.texCoord = QVector2D(0.0f, 0.0f);
//        }
//
//        vertices.push_back(vertex);
//    }
//
//    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
//        aiFace face = mesh->mFaces[i];
//        for(unsigned int j = 0; j < face.mNumIndices; j++)
//            indices.push_back(face.mIndices[j]);
//    }
//
//    // 处理材质
//    if(mesh->mMaterialIndex >= 0) {
//        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//        QVector<std::shared_ptr<Texture2D>> diffuseMaps = loadMaterialTextures(material,
//                                                           aiTextureType_DIFFUSE, "texture_diffuse");
//        textures.append(diffuseMaps);
//        QVector<std::shared_ptr<Texture2D>> specularMaps = loadMaterialTextures(material,
//                                                            aiTextureType_SPECULAR, "texture_specular");
//        textures.append(specularMaps);
//    }
//
//    return {vertices, indices, textures};
//}
//
//QVector<std::shared_ptr<Texture2D>> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, QString typeName) {
//    QVector<std::shared_ptr<Texture2D>> textures;
//    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        QString qStr = modelDirectory + "/" + QString::fromUtf8(str.C_Str());
//        qDebug() << "Load texture: " + qStr;
//        std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>();
//        texture->generate(qStr);
//        texture->type = typeName;
//        texture->path = qStr;
//
//        textures.push_back(texture);
//    }
//
//    return textures;
//}
