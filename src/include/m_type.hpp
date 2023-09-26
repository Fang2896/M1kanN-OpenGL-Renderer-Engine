//
// Created by fangl on 2023/9/26.
//

#ifndef M_TYPE_HPP
#define M_TYPE_HPP

#include <QString>


enum class ObjectType {
    Cube,
    Plane,
    Quad,
    Capsule,
    Sphere,
    Model,

};

enum class TextureType {
    Diffuse,
    Specular,
    Normal,
    Displacement,
    Occlusion,
    UNKNOWN
};

static QString objectTypeToString(ObjectType objType) {
    switch (objType) {
        case ObjectType::Cube:
            return "Cube";
        case ObjectType::Plane:
            return "Plane";
        case ObjectType::Quad:
            return "Quad";
        case ObjectType::Capsule:
            return "Capsule";
        case ObjectType::Sphere:
            return "Sphere";
        case ObjectType::Model:
            return "Model";
        default:
            return "Unknown";
    }
}

static QString textureTypeToString(TextureType texType) {
    switch (texType) {
        case TextureType::Diffuse:
            return "texture_diffuse";
        case TextureType::Specular:
            return "texture_specular";
        case TextureType::Normal:
            return "texture_normal";
        case TextureType::Displacement:
            return "texture_displacement";
        case TextureType::Occlusion:
            return "texture_occlusion";
        default:
            return "Unknown";
    }
}


static TextureType stringToTextureType(const QString& tex) {
    if (tex == "texture_diffuse") {
        return TextureType::Diffuse;
    } else if (tex == "texture_specular") {
        return TextureType::Specular;
    } else if (tex == "texture_normal") {
        return TextureType::Normal;
    } else if (tex == "texture_displacement") {
        return TextureType::Displacement;
    } else if (tex == "texture_occlusion") {
        return TextureType::Occlusion;
    } else {
        return TextureType::UNKNOWN;
    }
}

#endif  //M_TYPE_HPP
