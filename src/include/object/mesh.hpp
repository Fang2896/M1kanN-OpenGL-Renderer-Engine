//
// Created by fangl on 2023/9/23.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <cstddef>
#include <QVector>
#include <QVector3D>

#include "data_structures.hpp"
#include "gl_configure.hpp"
#include "utils/shader.hpp"
#include "utils/texture2d.hpp"

class Mesh {
   public:
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<std::shared_ptr<Texture2D>> textures;

    Mesh(QVector<Vertex> vertices, QVector<unsigned int> indices, QVector<std::shared_ptr<Texture2D>> textures);
    ~Mesh();

    void draw(const QString& shaderName);

   private:
    void setupMesh();

    GLuint VAO, VBO, EBO;
    GLFunctions_Core *glFunc;
};

#endif  //MESH_HPP
