//
// Created by fangl on 2023/9/24.
//

#ifndef SHAPEDATA_HPP
#define SHAPEDATA_HPP

#include "data_structures.hpp"


class ShapeData {
   public:
    // basic geometry
    // 1. Cube
    static const QVector<Vertex>& getCubeVertices() {
        static QVector<Vertex> cubeVertices;

        if(cubeVertices.isEmpty()) {
            QVector<float> data = {
                // Position          // Normal           // UV
                // Front face
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,

                // Back face
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,

                // Left face
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,

                // Right face
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

                // Top face
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

                // Bottom face
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            };

            for (int i = 0; i < data.size(); i += 8) {
                Vertex vertex;
                vertex.position = QVector3D(data[i], data[i + 1], data[i + 2]);
                vertex.normal = QVector3D(data[i + 3], data[i + 4], data[i + 5]);
                vertex.texCoord = QVector2D(data[i + 6], data[i + 7]);

                cubeVertices.push_back(vertex);
            }
        }

        return cubeVertices;
    }
    static const QVector<unsigned int>& getCubeIndices() {
        static QVector<unsigned int> cubeIndices = {
            // Front
            0, 1, 2,
            2, 3, 0,
            // Back
            4, 5, 6,
            6, 7, 4,
            // Left
            8, 9, 10,
            10, 11, 8,
            // Right
            12, 13, 14,
            14, 15, 12,
            // Top
            16, 17, 18,
            18, 19, 16,
            // Bottom
            20, 21, 22,
            22, 23, 20,
        };

        return cubeIndices;
    }

    // 2. Quad
    static const QVector<Vertex>& getQuadVertices() {
        static QVector<Vertex> quadVertices;

        if(quadVertices.isEmpty()) {
            QVector<float> data =
            {
                // positions       // normals         // textures
                -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            };

            for (int i = 0; i < data.size(); i += 8) {
                Vertex vertex;
                vertex.position = QVector3D(data[i], data[i + 1], data[i + 2]);
                vertex.normal = QVector3D(data[i + 3], data[i + 4], data[i + 5]);
                vertex.texCoord = QVector2D(data[i + 6], data[i + 7]);

                quadVertices.push_back(vertex);
            }
        }

        return quadVertices;
    }
    static const QVector<unsigned int>& getQuadIndices() {
        static QVector<unsigned int> quadIndices = {
            0, 1, 2,
            2, 3, 0
        };
        return quadIndices;
    }

    // 3. Plane
    static QVector<Vertex> getPlaneVertices(float width, float height) {
        QVector<Vertex> planeVertices{};
        // TODO: Add Plane Vertex
        return planeVertices;
    }
    static QVector<unsigned int> getPlaneIndices(float width, float height) {
        QVector<unsigned int> planeIndices = {
            // TODO: Add Plane Index
        };
        return planeIndices;
    }

    // 4. Capsule
    static const QVector<Vertex>& getCapsuleVertices(float radius, float height) {
        static QVector<Vertex> capsuleVertices;

        if(capsuleVertices.isEmpty()) {
            QVector<float> data = {
                // positions       // normals         // textures
                // TODO: Add Capsule Vertices
            };

            for (int i = 0; i < data.size(); i += 8) {
                Vertex vertex;
                vertex.position = QVector3D(data[i], data[i + 1], data[i + 2]);
                vertex.normal = QVector3D(data[i + 3], data[i + 4], data[i + 5]);
                vertex.texCoord = QVector2D(data[i + 6], data[i + 7]);

                capsuleVertices.push_back(vertex);
            }
        }

        return capsuleVertices;
    }
    static const QVector<unsigned int>& getCapsuleIndices(float radius, float height) {
        static QVector<unsigned int> capsuleIndices = {
            // TODO: Add Capsule Index
        };
        return capsuleIndices;
    }

    // 5. Sphere
    static const QVector<Vertex>& getSphereVertices(float radius) {
        static QVector<Vertex> sphereVertices;

        if(sphereVertices.isEmpty()) {
            QVector<float> data = {
                // positions       // normals         // textures
                // TODO: Add Sphere Vertices
            };

            for (int i = 0; i < data.size(); i += 8) {
                Vertex vertex;
                vertex.position = QVector3D(data[i], data[i + 1], data[i + 2]);
                vertex.normal = QVector3D(data[i + 3], data[i + 4], data[i + 5]);
                vertex.texCoord = QVector2D(data[i + 6], data[i + 7]);

                sphereVertices.push_back(vertex);
            }
        }

        return sphereVertices;
    }
    static const QVector<unsigned int>& getSphereIndices(float radius) {
        static QVector<unsigned int> sphereIndices = {
            // TODO: Add Sphere Indices
        };
        return sphereIndices;
    }

    ShapeData() = delete;

   private:

};


#endif  //SHAPEDATA_HPP
