//
// Created by fangl on 2023/9/23.
//

#include "mesh.hpp"


Mesh::Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<std::shared_ptr<Texture2D>> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

// 传到主渲染器里面，用于Draw
const GLuint& Mesh::getMeshVAO() {
    return VAO;
}

// 这个shader应该在其他地方以及初始化以及compile
void Mesh::draw(const Shader& shader) {
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;
    for(int i = 0; i < textures.size(); i++) {
        glFunc->glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
        // 获取纹理序号（diffuse_textureN 中的 N）
        QString number;
        QString name = textures[i]->type;
        if(name == "texture_diffuse")
            number = QString::number(diffuseNr++);
        else if(name == "texture_specular")
            number = QString::number(specularNr++);

        shader.setInteger("material." + name + number, i);
        glFunc->glBindTexture(GL_TEXTURE_2D, textures[i]->id);
    }
    glFunc->glActiveTexture(GL_TEXTURE0);

    // 绘制网格
    glFunc->glBindVertexArray(VAO);
    glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glFunc->glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if(!glFunc)
        qFatal("Require GLFunctions_Core to setUp mesh");

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

