//
// Created by fangl on 2023/9/23.
//

#ifndef MESH_HPP
#define MESH_HPP

#include <cstddef>
#include <QVector>
#include <QVector3D>

#include <utility>

#include "data_structures.hpp"
#include "gl_configure.hpp"
#include "utils/shader.hpp"
#include "utils/texture2d.hpp"


class Mesh {
   public:
    QVector<Vertex> vertices;
    QVector<unsigned int> indices;
    QVector<std::shared_ptr<Texture2D>> textures;

    Mesh(std::shared_ptr<Shader> sha,
         QVector<Vertex> vertices,
         QVector<unsigned int> indices,
         QVector<std::shared_ptr<Texture2D>> textures);
    ~Mesh();

    void updateData(QVector<Vertex> vertices,
                    QVector<unsigned int> indices,
                    QVector<std::shared_ptr<Texture2D>> textures);

    // 或许可以允许不同部位用不同的shader？
    void setShader(std::shared_ptr<Shader> sha);

    // draw configure
    void setDrawOutline(GLboolean drawState);
    void setTransform(QMatrix4x4 trans);
    void setMultiMesh(GLboolean isMulti);

    void draw();



   private:
    void setupMesh();
    void updateMesh();

    GLuint VAO{}, VBO{}, EBO{};
    GLFunctions_Core *glFunc;

    std::shared_ptr<Shader> shader;

    // draw configure
    GLboolean drawOutline;
    QMatrix4x4 transform;   // for outline
    GLboolean multiMesh;
};

#endif  //MESH_HPP
