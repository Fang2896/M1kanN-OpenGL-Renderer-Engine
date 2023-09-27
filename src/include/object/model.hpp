//
// Created by fangl on 2023/9/23.
//

#ifndef MODEL_HPP
#define MODEL_HPP

//#include <utility>
//
//#include <QVector>
//#include <QDebug>
//
//#include "assimp/Importer.hpp"
//#include "assimp/postprocess.h"
//#include "assimp/scene.h"
//
//#include "mesh.hpp"
//#include "object.hpp"
//#include "utils/texture2d.hpp"


//class Model : public Object {
//   public:
//    explicit Model(const char* mPath);
//    Model(QString objName, const char* mPath);
//
//    void init(QString shaderName) override;
//    void init() override;
//    void draw() override;   // 根据内部存储的shader名字来进行draw
//
//   private:
//    QVector<Mesh> meshes;
//
//    QString modelPath;
//    QString modelDirectory;
//
//    void loadModel(const QString &path);
//    void processNode(aiNode *node, const aiScene *scene);
//    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
//    QVector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
//                                         QString typeName);
//};

#endif  //MODEL_HPP
