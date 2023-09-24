//
// Created by fangl on 2023/9/24.
//

#include "object/object.hpp"


void Object::setShaderName(const QString& sName) {
    shaderName = sName;
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

