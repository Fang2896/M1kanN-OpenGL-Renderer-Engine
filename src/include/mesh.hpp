//
// Created by fangl on 2023/9/23.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <cstddef>
#include <QVector>
#include <QVector3D>

#include "shader.hpp"
#include "texture2d.hpp"
#include "gl_configure.hpp"


struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector3D texCoord;
};

class Mesh {
   public:
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<std::shared_ptr<Texture2D>> textures;

    Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<std::shared_ptr<Texture2D>> textures);
    const GLuint& getMeshVAO();
    void draw(const Shader& shader);

   private:
    void setupMesh();

    GLuint VAO, VBO, EBO;
    GLFunctions_Core *glFunc;
};

#endif  //MESH_HPP
