//
// Created by fangl on 2023/9/24.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <QMatrix4x4>
#include <QVector3D>
#include <utility>

#include "utils/shader.hpp"

// 作为shape类和model类的基函数
class Object {
   public:
    Object(QString type, QString objName, QString shaderName)
        : type(std::move(type)), objectName(std::move(objName)),
          shaderName(std::move(shaderName))
    {
        position = QVector3D{};
        rotation = QVector3D{};
        scale = QVector3D(1.0f, 1.0f, 1.0f);
    }

    Object(QString type, QString objName)
        : type(std::move(type)), objectName(std::move(objName)),
          shaderName("")
    {
        position = QVector3D{};
        rotation = QVector3D{};
        scale = QVector3D(1.0f, 1.0f, 1.0f);
    }

    ~Object() = default;

    virtual void init(QString objName) = 0;
    virtual void init() = 0;
    virtual void draw() = 0;

    // getter and setter
    QString getType();

    void setObjectName(QString objName);
    QString getObjectName();

    void setShaderName(QString sName);
    QString getShaderName();

    void setPosition(const QVector3D& pos);
    QVector3D getPosition();

    void setRotation(const QVector3D& rot); // 角度制度
    QVector3D getRotation();

    void setScale(const QVector3D& sca);
    void setScale(float sca);
    QVector3D getScale(const QVector3D& sca);

    QMatrix4x4 getTransform();

   private:
    const QString type;   // shape & model
    QString objectName;
    QString shaderName;

    // Transform
    QVector3D position;
    QVector3D rotation;
    QVector3D scale;
};

#endif  //OBJECT_HPP
