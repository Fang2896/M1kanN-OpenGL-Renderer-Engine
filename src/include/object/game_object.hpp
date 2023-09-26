//
// Created by fangl on 2023/9/26.
//

#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "mesh.hpp"
#include "shape_data.hpp"
#include "texture2d.hpp"
#include "resource_manager.hpp"
#include "data_structures.hpp"


class GameObject {
   public:
    GameObject();
    explicit GameObject(ObjectType type, const QString& disName = "GameObject");
    GameObject(ObjectType type, const QString& mPath, const QString& disName = "GameObject");
    ~GameObject();

    void draw();

    void loadShape(ObjectType t, int width=10, int height=10);   // only for non-model shape
    void loadModel(const QString& mPath); // only for model
    void loadShader(const QString& vertPath,
                    const QString& fragPath,
                    const QString& geoPath = nullptr);


    void setMaterial(Material mat);
    void setDiffuseTexture(std::shared_ptr<Texture2D> tex);
    void setSpecularTexture(std::shared_ptr<Texture2D> tex);
    void setAmbientColor(QVector3D col);
    void setDiffuseColor(QVector3D col);
    void setSpecularColor(QVector3D col);
    void setAmbientOcclusion(float ab);

    ObjectType getType();
    QString getShaderName();

    QMatrix4x4 getTransform();
    QVector3D getPosition();
    QVector3D getRotation();
    QVector3D getScale();

    void setTransform(QMatrix4x4 trans);
    void setPosition(QVector3D pos);
    void setRotation(QVector3D rot);
    void setScale(QVector3D sca);

    [[nodiscard]] GLuint getObjectID() const;
    static GLuint getObjectTotalNumber();

   public:
    QString displayName;

   private:
    void updateTransform();

   private:
    static GLuint gameObjectCounter;
    GLuint objectID;

    // basic info
    ObjectType type;
    QString modelPath;  // optional, only for model type

    std::shared_ptr<Shader> shader;
    QString shaderName;
    Material material;

    // model: multiple, shape: single
    QVector<std::shared_ptr<Mesh>> meshes;

    // transforms info
    QVector3D position;
    QVector3D rotation;
    QVector3D scale;
    QMatrix4x4 transform;

};

#endif  //GAME_OBJECT_HPP
