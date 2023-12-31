//
// Created by fangl on 2023/9/24.
//

#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include "m_type.hpp"
#include "texture2d.hpp"


const static int PostProcessingScreenWidth = 800;
const static int PostProcessingScreenHeight = 600;


struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;

    Vertex() = default;
    Vertex(QVector3D pos, QVector3D nor, QVector2D tex)
        : position(pos), normal(nor), texCoord(tex) {}
};

struct DirectLight {
    QVector3D direction; // Light direction
    QVector3D ambientColor;     // Light color
    QVector3D diffuseColor;     // Light color
    QVector3D specularColor;     // Light color
    float intensity; // Light intensity

    DirectLight(QVector3D dir, QVector3D aCol, QVector3D dCol, QVector3D sCol,float inten)
        : direction(dir),
          ambientColor(aCol),
          diffuseColor(dCol),
          specularColor(sCol),
          intensity(inten) {}

    DirectLight()
        : direction(QVector3D(1,1,1).normalized()),
          ambientColor({0.4f, 0.4f, 0.4f}),
          diffuseColor({1.0f, 1.0f, 1.0f}),
          specularColor({0.9f, 0.9f, 0.9f}),
          intensity(1.0f) {}
};

struct PointLight {
    QVector3D position; // Light direction
    QVector3D ambientColor;     // Light color
    QVector3D diffuseColor;     // Light color
    QVector3D specularColor;     // Light color
    float intensity; // Light intensity
    float falloff;

    PointLight(QVector3D pos, QVector3D aCol, QVector3D dCol, QVector3D sCol, float inten, float fal)
        : position(pos),
          ambientColor(aCol),
          diffuseColor(dCol),
          specularColor(sCol),
          intensity(inten), falloff(fal) {}

    PointLight()
        : position({4,4,4}),
          ambientColor({0.1f, 0.1f, 0.1f}),
          diffuseColor({1.0f, 1.0f, 1.0f}),
          specularColor({0.9f, 0.9f, 0.9f}),
          intensity(1.0f), falloff(0.001f) {}
};

struct SpotLight {
    QVector3D position; // Light direction
    QVector3D direction;
    QVector3D ambientColor;     // Light color
    QVector3D diffuseColor;     // Light color
    QVector3D specularColor;     // Light color
    float intensity; // Light intensity
    float falloff;

    SpotLight(QVector3D pos, QVector3D dir, QVector3D aCol, QVector3D dCol, QVector3D sCol, float inten, float fal)
        : position(pos), direction(dir),
          ambientColor(aCol),
          diffuseColor(dCol),
          specularColor(sCol),
          intensity(inten), falloff(fal) {}

    SpotLight()
        : position(0,4,0), direction(0,-1,0),
          ambientColor({0.1f, 0.1f, 0.1f}),
          diffuseColor({1.0f, 1.0f, 1.0f}),
          specularColor({0.9f, 0.9f, 0.9f}),
          intensity(1), falloff(0.001f) {}
};

struct Material {
    float shininess;
    // 后面可能会加更多？
    std::shared_ptr<Texture2D> texture_diffuse1;
    std::shared_ptr<Texture2D> texture_specular1;

    QVector3D ambientColor;
    QVector3D diffuseColor;
    QVector3D specularColor;

    float ambientOcclusion;

    Material(float shi, QVector3D aCol, QVector3D dCol, QVector3D sCol,float amb)
        : shininess(shi),
          texture_diffuse1(nullptr),
          texture_specular1(nullptr),
          ambientColor(aCol),
          diffuseColor(dCol),
          specularColor(sCol),
          ambientOcclusion(amb) {}

    Material() : shininess(0.4),
          texture_diffuse1(nullptr),
          texture_specular1(nullptr),
          ambientColor({0.0,0.1,0.06}),
          diffuseColor({ 0.0,0.50980392,0.50980392}),
          specularColor({0.50196078,0.50196078,0.50196078}),
          ambientOcclusion(0.25) {}
};


/*
Name	Ambient	                Diffuse	                Specular	                Shininess
emerald	0.0215	0.1745	0.0215	0.07568	0.61424	0.07568	0.633	0.727811	0.633	0.6
jade	0.135	0.2225	0.1575	0.54	0.89	0.63	0.316228	0.316228	0.316228	0.1
obsidian	0.05375	0.05	0.06625	0.18275	0.17	0.22525	0.332741	0.328634	0.346435	0.3
pearl	0.25	0.20725	0.20725	1	0.829	0.829	0.296648	0.296648	0.296648	0.088
ruby	0.1745	0.01175	0.01175	0.61424	0.04136	0.04136	0.727811	0.626959	0.626959	0.6
turquoise	0.1	0.18725	0.1745	0.396	0.74151	0.69102	0.297254	0.30829	0.306678	0.1
brass	0.329412	0.223529	0.027451	0.780392	0.568627	0.113725	0.992157	0.941176	0.807843	0.21794872
bronze	0.2125	0.1275	0.054	0.714	0.4284	0.18144	0.393548	0.271906	0.166721	0.2
chrome	0.25	0.25	0.25	0.4	0.4	0.4	0.774597	0.774597	0.774597	0.6
copper	0.19125	0.0735	0.0225	0.7038	0.27048	0.0828	0.256777	0.137622	0.086014	0.1
gold	0.24725	0.1995	0.0745	0.75164	0.60648	0.22648	0.628281	0.555802	0.366065	0.4
silver	0.19225	0.19225	0.19225	0.50754	0.50754	0.50754	0.508273	0.508273	0.508273	0.4
black plastic	0.0	0.0	0.0	0.01	0.01	0.01	0.50	0.50	0.50	.25
cyan plastic	0.0	0.1	0.06	0.0	0.50980392	0.50980392	0.50196078	0.50196078	0.50196078	.25
green plastic	0.0	0.0	0.0	0.1	0.35	0.1	0.45	0.55	0.45	.25
red plastic	0.0	0.0	0.0	0.5	0.0	0.0	0.7	0.6	0.6	.25
white plastic	0.0	0.0	0.0	0.55	0.55	0.55	0.70	0.70	0.70	.25
yellow plastic	0.0	0.0	0.0	0.5	0.5	0.0	0.60	0.60	0.50	.25
black rubber	0.02	0.02	0.02	0.01	0.01	0.01	0.4	0.4	0.4	.078125
cyan rubber	0.0	0.05	0.05	0.4	0.5	0.5	0.04	0.7	0.7	.078125
green rubber	0.0	0.05	0.0	0.4	0.5	0.4	0.04	0.7	0.04	.078125
red rubber	0.05	0.0	0.0	0.5	0.4	0.4	0.7	0.04	0.04	.078125
white rubber	0.05	0.05	0.05	0.5	0.5	0.5	0.7	0.7	0.7	.078125
yellow rubber	0.05	0.05	0.0	0.5	0.5	0.4	0.7	0.7	0.04	.078125
*/

#endif  //DATA_STRUCTURES_HPP
