//
// Created by fangl on 2023/9/23.
//

#include <utility>

#include "object/mesh.hpp"


Mesh::Mesh(std::shared_ptr<Shader> sha, QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<std::shared_ptr<Texture2D>> textures) {
    this->multiMesh = GL_FALSE;
    this->shader = std::move(sha);
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);

    this->drawOutline = GL_FALSE;

    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if(!glFunc)
        qFatal("Require GLFunctions_Core to setUp mesh");

    setupMesh();
}

// TODO: 这里到底要不要delete VAO等？
Mesh::~Mesh() = default;

void Mesh::updateData(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<std::shared_ptr<Texture2D>> textures) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);

    updateMesh();
}

void Mesh::setShader(std::shared_ptr<Shader> sha) {
    this->shader = std::move(sha);
}

void Mesh::setDrawOutline(GLboolean drawState) {
    this->drawOutline = drawState;
}

void Mesh::setTransform(QMatrix4x4 trans) {
    this->transform = trans;
}

void Mesh::setMultiMesh(GLboolean isMulti) {
    this->multiMesh = isMulti;

    for(auto &t : textures) {
        // 确保即使是透明的，也要是Repeat，因为多mesh，多material模型会让vt超过1.
        t->setWrapMode(QOpenGLTexture::Repeat, QOpenGLTexture::Repeat);
    }
}

void Mesh::draw() {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    /*============ outline logic ============*/
    if(drawOutline) {
        glFunc->glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glFunc->glStencilMask(0xFF);
    } else {
        glFunc->glStencilMask(0x00);
        shader->setBool("enableOutline", false);
    }
    /*============ outline logic ============*/

    shader->use();
    for(int i = 0; i < textures.size(); i++) {
        // 在绑定之前激活相应的纹理单元
        glFunc->glActiveTexture(GL_TEXTURE0 + i);
        // 获取纹理序号（diffuse_textureN 中的 N）
        QString number;
        QString name = textureTypeToString(textures[i]->type);
        if(name == "texture_diffuse") {
            number = QString::number(diffuseNr++);
            shader->setBool("useDiffuseTexture", true);
        }
        else if(name == "texture_specular") {
            number = QString::number(specularNr++);
            shader->setBool("useSpecularTexture", true);
        } else
            qFatal("Type of Texture is Not Support!");

        // 这里的material是model的material，当前仅仅有贴图
        shader->setInteger("material." + name + number, i);
        glFunc->glBindTexture(GL_TEXTURE_2D, textures[i]->id);
    }

    // 1st: 绘制网格
    shader->setBool("enableOutline", false);
    glFunc->glBindVertexArray(VAO);
    glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glFunc->glBindVertexArray(0);

    /*============ outline logic ============*/
    // 2nd draw the outline
    if(drawOutline) {
        glFunc->glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glFunc->glStencilMask(0x00);
        glFunc->glDisable(GL_DEPTH_TEST);

        // 更新M矩阵
        shader->setBool("enableOutline", true);
        QMatrix4x4 outLineTrans = this->transform;
        outLineTrans.scale(1.05f);
        shader->setMatrix4f("model", outLineTrans);   // Model 要传进来

        glFunc->glBindVertexArray(VAO);
        glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glFunc->glBindVertexArray(0);

        glFunc->glStencilMask(0xFF);
        glFunc->glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glFunc->glEnable(GL_DEPTH_TEST);
        // glFunc->glDisable(GL_STENCIL_TEST);
    } else {
        // TODO: 还是有outline深度绘制错误
        glFunc->glStencilMask(0xFF);    // 如果不加这一行。outline会显示错误
    }
    /*============ outline logic ============*/

    shader->release();
}

void Mesh::setupMesh() {
    glFunc->glGenVertexArrays(1, &VAO);
    glFunc->glGenBuffers(1, &VBO);
    glFunc->glGenBuffers(1, &EBO);

    glFunc->glBindVertexArray(VAO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFunc->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                         &vertices[0], GL_STATIC_DRAW);

    glFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex position
    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex), (void*)0);
    // vertex normal
    glFunc->glEnableVertexAttribArray(1);
    glFunc->glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,
                                  sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex uv
    glFunc->glEnableVertexAttribArray(2);
    glFunc->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glFunc->glBindVertexArray(0);
}

void Mesh::updateMesh() {
    if(!VAO || !VBO || !EBO ) {
        qFatal("VAO | VBO | EBO is Empty");
    }

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFunc->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                         &vertices[0], GL_STATIC_DRAW);

    glFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                         &indices[0], GL_STATIC_DRAW);

    qDebug("Update Mesh Success");
}
