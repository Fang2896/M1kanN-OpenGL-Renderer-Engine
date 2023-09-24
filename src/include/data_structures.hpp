//
// Created by fangl on 2023/9/24.
//

#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>


struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
};

struct DirectionalLight {
    QVector3D direction; // Light direction
    QVector3D color;     // Light color
    float intensity; // Light intensity

    DirectionalLight(QVector3D dir, QVector3D col, float inten)
        : direction(dir), color(col), intensity(inten) {}
};

struct PointLight {
    QVector3D position; // Light direction
    QVector3D color;     // Light color
    float intensity; // Light intensity
    float falloff;

    PointLight(QVector3D pos, QVector3D col, float inten, float fal)
        : position(pos), color(col), intensity(inten), falloff(fal) {}
};

struct SpotLight {
    QVector3D position; // Light direction
    QVector3D direction;
    QVector3D color;     // Light color
    float intensity; // Light intensity
    float falloff;

    SpotLight(QVector3D pos, QVector3D dir, QVector3D col, float inten, float fal)
        : position(pos), direction(dir), color(col),
          intensity(inten), falloff(fal) {}
};

struct Material {
    float shininess;
    float metallic;
    float roughness;
    QVector3D baseColor;
    float ambientOcclusion;

    Material(float shi, float meta, float rough, QVector3D col, float amb)
        : shininess(shi), metallic(meta), roughness(rough),
          baseColor(col), ambientOcclusion(amb) {}

    Material()
        : shininess(128), metallic(0.95), roughness(0.5),
          baseColor({1.0f,0.2f,0.2f}), ambientOcclusion(0.0f) {}
};



#endif  //DATA_STRUCTURES_HPP
