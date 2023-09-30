//
// Created by fangl on 2023/9/26.
//

#include <utility>

#include "object/game_object.hpp"


GLuint GameObject::gameObjectCounter = 0;

GameObject::GameObject()
    : display(GL_TRUE), displayName("GameObject"), objectID(gameObjectCounter++),
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
        ShapeData::getCubeVertices(),
        ShapeData::getCubeIndices(),
        vecTextures);

    meshes = QVector<std::shared_ptr<Mesh>>{cubeMesh};

    this->position = QVector3D(0.0f, 0.0f, 0.0f);
    this->rotation = QVector3D(0.0f, 0.0f, 0.0f);
    this->scale    = QVector3D(1.0f, 1.0f, 1.0f);
    this->transform.setToIdentity();
    this->transform.translate(position);
    this->transform.rotate(QQuaternion::fromEulerAngles(rotation));
    this->transform.scale(scale);
    shader->use();
    shader->setMatrix4f("model", this->transform);
    shader->release();
}

GameObject::GameObject(ObjectType type, const QString& disName)
    : GameObject()
{
    this->displayName = disName;
    this->type = type;

    if(type == ObjectType::Model) {
        qDebug("==>Type is Model, Please give model path using loadModel<==");
    }

    loadShape(type);
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
}

GameObject::~GameObject() {
    meshes.clear();
    // 可能要通知主界面？需要删除显示的list

}

void GameObject::draw() {
    if(!display) {
        return;
    }

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
        case ObjectType::Cube:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getCubeVertices(),
                                              ShapeData::getCubeIndices(),
                                              vecTextures));
            break;
        case ObjectType::Plane:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getPlaneVertices(width, height),
                                                 ShapeData::getPlaneIndices(width, height),
                                                 vecTextures));
            break;
        case ObjectType::Quad:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getQuadVertices(),
                                                 ShapeData::getQuadIndices(),
                                                 vecTextures));
            break;
        case ObjectType::Capsule:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getCapsuleVertices(width / 2.0f, height),
                                                 ShapeData::getCapsuleIndices(width / 2.0f, height),
                                                 vecTextures));
            break;
        case ObjectType::Sphere:
            meshes.append(std::make_shared<Mesh>(shader, ShapeData::getSphereVertices(width / 2.0f),
                                                 ShapeData::getSphereIndices(width / 2.0f),
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
    }

    qDebug("Load Model Finished");
}

void GameObject::loadShader(const QString& vertPath, const QString& fragPath, const QString& geoPath) {
    ResourceManager::loadShader(shaderName, vertPath, fragPath, geoPath);
    shader = ResourceManager::getShader(shaderName);
    ResourceManager::updateMaterialInShader(shaderName, material);
    shader->use();
    shader->setMatrix4f("model", transform);
    shader->release();

    for(auto & m : meshes) {
        m->setShader(shader);
    }
}

void GameObject::loadDiffuseTexture(const QString& tPath) {
    if(type == ObjectType::Model && meshes.size() > 1) {
        qDebug("Warning! Loaded Model Type Can't Be Loaded Diffuse Texture");
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

    // 清除原来的diffuse然后赋值
    meshes[0]->textures.append(material.texture_diffuse1);

    shader->use();
    shader->setBool("useDiffuseTexture", true);
    shader->setBool("material.texture_diffuse1", material.texture_diffuse1->getTextureID());
    shader->release();
}

void GameObject::loadSpecularTexture(const QString& tPath) {
    if(type == ObjectType::Model) {
        qDebug("Warning! Model Type Can't Be Loaded Specular Texture");
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

void GameObject::setDiffuseTexture(const QString& tPath) {
    std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
    tex->generate(tPath);
    tex->type = TextureType::Diffuse;
    tex->path = tPath;

    for(auto& m : meshes) {
        m->textures.append(tex);
    }

    this->material.texture_diffuse1 = tex;
    shader->use();
    shader->setBool("useDiffuseTexture", true);
    shader->setInteger("material.texture_diffuse1", tex->getTextureID());
    shader->release();
}

void GameObject::setSpecularTexture(const QString& tPath) {
    std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
    tex->generate(tPath);
    tex->type = TextureType::Specular;
    tex->path = tPath;

    // TODO: 如果是多texture多mesh，咋办？
    for(auto& m : meshes) {
        m->textures.append(tex);
    }

    this->material.texture_specular1 = tex;
    shader->use();
    shader->setBool("useSpecularTexture", true);
    shader->setInteger("material.texture_specular1", tex->getTextureID());
    shader->release();
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

void GameObject::setTransform(QMatrix4x4 trans) {
    this->transform = trans;
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


