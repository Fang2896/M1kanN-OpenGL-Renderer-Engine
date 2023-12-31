﻿//
// Created by fangl on 2023/9/26.
//

#include <utility>

#include "object/game_object.hpp"


GLuint GameObject::gameObjectCounter = 0;

GameObject::GameObject()
    : display(GL_TRUE), drawOutline(GL_FALSE), containTransparencyTexture(GL_FALSE),
      displayName("GameObject"), objectID(gameObjectCounter++),
      type(ObjectType::Cube), modelPath(""),
      shader(), material()
{
    shaderName = "shader_" + QString::number(objectID);
    ResourceManager::loadShader(shaderName,
                                ":/shaders/assets/shaders/defaultShader.vert",
                                ":/shaders/assets/shaders/defaultShader.frag");
    shader = ResourceManager::getShader(shaderName);
    ResourceManager::updateMaterialInShader(shaderName, material);

    QVector<std::shared_ptr<Texture2D>> vecTextures{};
    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(
        shader,
        ShapeData::getUnitCubeVertices(),
        ShapeData::getUnitCubeIndices(),
        vecTextures);

    meshes = QVector<std::shared_ptr<Mesh>>{cubeMesh};

    this->position = QVector3D(0.0f, 0.0f, 0.0f);
    this->rotation = QVector3D(0.0f, 0.0f, 0.0f);
    this->scale    = QVector3D(1.0f, 1.0f, 1.0f);
    this->transform.setToIdentity();
    this->transform.translate(position);
    this->transform.rotate(QQuaternion::fromEulerAngles(rotation));
    this->transform.scale(scale);

    for(auto& m : meshes) {
        m->setTransform(this->transform);
    }

    shader->use();
    shader->setMatrix4f("model", this->transform);
    shader->release();
}

GameObject::GameObject(ObjectType type, float width, float height, const QString& disName)
    : GameObject()
{
    this->displayName = disName;
    this->type = type;

    if(type == ObjectType::Model) {
        qDebug("==>Type is Model, Please give model path using loadModel<==");
    }

    loadShape(type, width, height);
}

GameObject::GameObject(const QString& mPath, const QString& disName)
    : GameObject()
{
    this->type = ObjectType::Model;
    this->displayName = UtilAlgorithms::getFileNameFromPath(mPath);
    this->modelPath = mPath;

    if(type != ObjectType::Model) {
        qDebug("==>Type is NOT Model, Now Change to Model and Load<==");
    }
    meshes.clear();
    loadModel(mPath);
    if(meshes.size() > 1) {
        for(auto &m : meshes) {
            m->setMultiMesh(GL_TRUE);
        }
    }
}

GameObject::~GameObject() {
    meshes.clear();
    // 可能要通知主界面？需要删除显示的list

}

void GameObject::draw() {
    if(!display) {
        return;
    }

    shader->use();
    if(meshes.size() > 1) {
        shader->setBool("isMultiMeshModel", GL_TRUE);
    } else {
        shader->setBool("isMultiMeshModel", GL_FALSE);
    }
    shader->release();

    for(auto & m : meshes) {
        m->draw();
    }
}

void GameObject::loadShape(ObjectType t, float width, float height) {
    // width or diameter
    this->type = t;
    meshes.clear();
    QVector<std::shared_ptr<Texture2D>> vecTextures{};

    switch (t) {
        case ObjectType::UnitCube:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getUnitCubeVertices(),
                                              ShapeData::getUnitCubeIndices(),
                                              vecTextures));
            break;
        case ObjectType::Cube:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getCubeVertices(static_cast<int>(width)),
                                                 ShapeData::getCubeIndices(static_cast<int>(width)),
                                                 vecTextures));
            break;
        case ObjectType::Plane:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getPlaneVertices(static_cast<int>(width), static_cast<int>(height)),
                                                 ShapeData::getPlaneIndices(static_cast<int>(width), static_cast<int>(height)),
                                                 vecTextures));
            break;
        case ObjectType::Quad:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getQuadVertices(),
                                                 ShapeData::getQuadIndices(),
                                                 vecTextures));
            break;
        case ObjectType::Capsule:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getCapsuleVertices(static_cast<float>(width), static_cast<float>(height)),
                                                 ShapeData::getCapsuleIndices(static_cast<float>(width), static_cast<float>(height)),
                                                 vecTextures));
            break;
        case ObjectType::Sphere:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getSphereVertices(width, static_cast<int>(height)),
                                                 ShapeData::getSphereIndices(width, static_cast<int>(height)),
                                                 vecTextures));
            break;
        case ObjectType::Model:
            qDebug("==>Type is Model, Please use loadModel<==");
            break;
        default:
            qFatal("TYPE WRONG!");
    }

    qDebug("Load Shape Finished");
}

void GameObject::loadModel(const QString& mPath) {
    this->type = ObjectType::Model;
    this->meshes = ResourceManager::loadModel(mPath);

    for(auto & m : meshes) {
        m->setShader(shader);
        m->setTransform(transform);
    }

    if(meshes.size() > 1) {
        for(auto & m : meshes) {
            m->setMultiMesh(GL_TRUE);
        }
    }

    qDebug("Load Model Finished");
}

//void GameObject::loadShader(const QString& vertPath, const QString& fragPath, const QString& geoPath) {
//    ResourceManager::loadShader(shaderName, vertPath, fragPath, geoPath);
//    shader = ResourceManager::getShader(shaderName);
//    ResourceManager::updateMaterialInShader(shaderName, material);
//    shader->use();
//    shader->setMatrix4f("model", transform);
//    shader->release();
//
//    for(auto & m : meshes) {
//        m->setShader(shader);
//    }
//}
//
//void GameObject::loadShader(const QString& sName) {
//    auto tempShader = ResourceManager::getShader(sName);
//    if(tempShader == nullptr) {
//        return;
//    }
//
//    ResourceManager::updateMaterialInShader(shaderName, material);
//    shader->use();
//    shader->setMatrix4f("model", transform);
//    shader->release();
//
//    for(auto & m : meshes) {
//        m->setShader(shader);
//    }
//}
//
//void GameObject::loadShader(ShaderType sType) {
//    std::shared_ptr<Shader> tempShader;
//    switch (sType) {
//        case (ShaderType::Default):
//            tempShader = ResourceManager::getShader("defaultShader");
//            shaderName = "defaultShader";
//            break;
//        case (ShaderType::Reflection):
//            if (meshes.count() != 1) {
//                qDebug(
//                    "The Number of Meshes is not equal to 1. Can't Load Reflection shader");
//                return;
//            }
//            tempShader = ResourceManager::getShader("reflectionShader");
//            shaderName = "reflectionShader";
//            break;
//        case (ShaderType::Refraction):
//            if (meshes.count() != 1) {
//                qDebug(
//                    "The Number of Meshes is not equal to 1. Can't Load Refraction shader");
//                return;
//            }
//            shaderName = "refractionShader";
//            tempShader = ResourceManager::getShader("refractionShader");
//            break;
//    }
//
//    if(tempShader == nullptr) {
//        return;
//    }
//
//    ResourceManager::updateMaterialInShader(shaderName, material);
//    shader->use();
//    shader->setMatrix4f("model", transform);
//    shader->release();
//
//    for(auto & m : meshes) {
//        m->setShader(shader);
//    }
//
//}

void GameObject::loadDiffuseTexture(const QString& tPath) {
    if(meshes.size() > 1) {
        qDebug("Warning! Multiple Material Model Type Can't Be Loaded Specular Texture");
        return;
    }
    if(meshes.isEmpty()) {
        qDebug("You must assign a Mesh to loaded a diffuse texture!");
    }
    // 这里直接覆盖掉原来的texture
    material.texture_diffuse1 = std::make_shared<Texture2D>();
    material.texture_diffuse1->generate(tPath);
    material.texture_diffuse1->type = TextureType::Diffuse;
    material.texture_diffuse1->path = tPath;

    // 清除原来的diffuse然后赋值
    auto& tempVec = meshes[0]->textures;
    tempVec.erase(std::remove_if(tempVec.begin(), tempVec.end(),
                                 [](const std::shared_ptr<Texture2D> &tex) {
                                     return tex->type == TextureType::Diffuse;
                                 }), tempVec.end());

    meshes[0]->textures.append(material.texture_diffuse1);

    containTransparencyTexture = GL_FALSE;
    for(auto &t : meshes[0]->textures) {
        if(t->transparent) {
            containTransparencyTexture = GL_TRUE;
            break;
        }
    }

    shader->use();
    shader->setBool("useDiffuseTexture", true);
    shader->setInteger("material.texture_diffuse1", material.texture_diffuse1->getTextureID());
    shader->release();
}

void GameObject::loadSpecularTexture(const QString& tPath) {
    if(meshes.size() > 1) {
        qDebug("Warning! Multiple Material Model Type Can't Be Loaded Specular Texture");
        return;
    }
    if(meshes.isEmpty()) {
        qDebug("You must assign a Mesh to loaded a specular texture!");
    }
    // 这里直接覆盖掉原来的texture
    material.texture_specular1 = std::make_shared<Texture2D>();
    material.texture_specular1->generate(tPath);
    material.texture_specular1->type = TextureType::Specular;
    material.texture_specular1->path = tPath;

    // 清除原来的specular然后赋值
    auto& tempVec = meshes[0]->textures;
    tempVec.erase(std::remove_if(tempVec.begin(), tempVec.end(),
                                 [](const std::shared_ptr<Texture2D> &tex) {
                                     return tex->type == TextureType::Specular;
                                 }), tempVec.end());
    tempVec.append(material.texture_specular1);

    shader->use();
    shader->setBool("useSpecularTexture", true);
    shader->setInteger("material.texture_specular1", material.texture_specular1->getTextureID());
    shader->release();
}

// only for shape or pure model without texture, not model
void GameObject::setMaterial(Material mat) {
    if(meshes.size() != 1) {
        qDebug("Multiple Model Type or Empty Shape set material");
    }

    QVector<std::shared_ptr<Texture2D>> texVec;
    texVec.clear();
    if(mat.texture_diffuse1 != nullptr) {
        mat.texture_diffuse1->type = TextureType::Diffuse;
        texVec.append(mat.texture_diffuse1);
    }
    if(mat.texture_specular1 != nullptr) {
        mat.texture_specular1->type = TextureType::Specular;
        texVec.append(mat.texture_specular1);
    }

    if(!texVec.isEmpty()) {
        for(auto& m : meshes) {
            m->textures = texVec;
        }
    }

    this->material = std::move(mat);
    ResourceManager::updateMaterialInShader(shaderName, material);
}

void GameObject::setAmbientColor(QVector3D col) {
    this->material.ambientColor = col;
    shader->use();
    shader->setVector3f("material.ambientColor", col);
    shader->release();
}

void GameObject::setDiffuseColor(QVector3D col) {
    this->material.diffuseColor = col;
    shader->use();
    shader->setVector3f("material.diffuseColor", col);
    shader->release();
}

void GameObject::setSpecularColor(QVector3D col) {
    this->material.specularColor = col;
    shader->use();
    shader->setVector3f("material.specularColor", col);
    shader->release();
}

void GameObject::setAmbientOcclusion(float ab) {
    this->material.ambientOcclusion = ab;
    shader->use();
    shader->setFloat("material.ambientOcclusion", ab);
    shader->release();
}

void GameObject::setReflection(GLboolean isReflec) {
    shader->use();
    shader->setBool("isReflection", isReflec);
    shader->setBool("isRefraction", false);
    shader->setBool("isFresnel", false);
    shader->setInteger("skybox", 31);    // 31作为默认的天空盒参数？
    shader->release();
}

void GameObject::setRefraction(GLboolean isRefrac) {
    shader->use();
    shader->setBool("isRefraction", isRefrac);
    shader->setBool("isReflection", false);
    shader->setBool("isFresnel", false);
    shader->setInteger("skybox", 31);    // 31作为默认的天空盒参数？
    shader->release();
}

void GameObject::setFresnel(GLboolean isFre) {
    shader->use();
    shader->setBool("isRefraction", false);
    shader->setBool("isReflection", false);
    shader->setBool("isFresnel", isFre);
    shader->setInteger("skybox", 31);    // 31作为默认的天空盒参数？
    shader->release();
}

ObjectType GameObject::getType() {
    return this->type;
}

QString GameObject::getShaderName() {
    return this->shaderName;
}

int GameObject::getMeshCount() {
    return meshes.size();
}

const Material& GameObject::getMaterial() {
    return material;
}

GLboolean GameObject::getVisible() {
    return display;
}

GLboolean GameObject::getDrawOutline() {
    return drawOutline;
}

QMatrix4x4 GameObject::getTransform() {
    return this->transform;
}

QVector3D GameObject::getPosition() {
    return this->position;
}

QVector3D GameObject::getRotation() {
    return this->rotation;
}

QVector3D GameObject::getScale() {
    return this->scale;
}

void GameObject::setVisible(GLboolean visState) {
    this->display = visState;
}

void GameObject::setDrawOutline(GLboolean drawState) {
    this->drawOutline = drawState;
    for(auto &m : meshes) {
        m->setTransform(transform);
        m->setDrawOutline(drawState);
    }
}

void GameObject::setTransform(QMatrix4x4 trans) {
    this->transform = trans;

    for(auto& m : meshes) {
        m->setTransform(transform);
    }

    shader->use();
    shader->setMatrix4f("model", trans);
    shader->release();
}

void GameObject::setPosition(QVector3D pos) {
    this->position = pos;
    updateTransform();
}
void GameObject::setRotation(QVector3D rot) {
    this->rotation = rot;
    updateTransform();
}

void GameObject::setScale(QVector3D sca) {
    this->scale = sca;
    updateTransform();
}

void GameObject::setScale(float sca) {
    this->scale = QVector3D(sca, sca, sca);
    updateTransform();
}

void GameObject::updateTransform() {
    transform.setToIdentity();
    transform.translate(position);
    transform.rotate(QQuaternion::fromEulerAngles(rotation));
    transform.scale(scale);

    for(auto& m : meshes) {
        m->setTransform(transform);
    }

    shader->use();
    shader->setMatrix4f("model", transform);
    shader->release();
}

GLuint GameObject::getObjectID() const {
    return objectID;
}

GLuint GameObject::getObjectTotalNumber() {
    return gameObjectCounter;
}


