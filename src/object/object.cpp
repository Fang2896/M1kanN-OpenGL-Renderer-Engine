//
// Created by fangl on 2023/9/24.
//

#include <utility>

#include "object/object.hpp"


QString Object::getType() {
    return type;
}

void Object::setObjectName(QString objName) {
    objectName = std::move(objName);
}

QString Object::getObjectName() {
    return objectName;
}

void Object::setShaderName(QString sName) {
    shaderName = std::move(sName);
}

QString Object::getShaderName() {
    return shaderName;
}

void Object::setPosition(const QVector3D& pos) {
    position = pos;
}

QVector3D Object::getPosition() {
    return position;
}

void Object::setRotation(const QVector3D& rot) {
    rotation = rot;
}

QVector3D Object::getRotation() {
    return rotation;
}

void Object::setScale(const QVector3D& sca) {
    scale = sca;
}

void Object::setScale(float sca) {
    scale = QVector3D(sca, sca, sca);
}

QVector3D Object::getScale(const QVector3D& sca) {
    return scale;
}

QMatrix4x4 Object::getTransform() {
    QMatrix4x4 transform;
    transform.setToIdentity();
    transform.translate(position);
    transform.rotate(QQuaternion::fromEulerAngles(rotation));
    transform.scale(scale);

    return transform;
}

