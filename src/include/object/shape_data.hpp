//
// Created by fangl on 2023/9/24.
//

#ifndef SHAPEDATA_HPP
#define SHAPEDATA_HPP

#include "data_structures.hpp"


class ShapeData {
   public:
    // basic geometry
    // 1. Unit Cube
    static const QVector<Vertex>& getUnitCubeVertices() {
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
    static const QVector<unsigned int>& getUnitCubeIndices() {
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

    // 2. Cube
    static QVector<Vertex> getCubeVertices(int width) {
        QVector<Vertex> vertices;
        vertices.append(getPlaneVertices(width,width,Direction::UP));
        vertices.append(getPlaneVertices(width,width,Direction::DOWN));
        vertices.append(getPlaneVertices(width,width,Direction::LEFT));
        vertices.append(getPlaneVertices(width,width,Direction::RIGHT));
        vertices.append(getPlaneVertices(width,width,Direction::FRONT));
        vertices.append(getPlaneVertices(width,width,Direction::BACK));

        return vertices;
    }
    static QVector<unsigned int> getCubeIndices(int width) {
        QVector<unsigned int> indices;
        int indexStep = (width+1) * (width+1);

        indices.append(getPlaneIndices(width, width, Direction::UP, 0));
        indices.append(getPlaneIndices(width, width, Direction::DOWN, indexStep * 1));
        indices.append(getPlaneIndices(width, width, Direction::LEFT, indexStep * 2));
        indices.append(getPlaneIndices(width, width, Direction::RIGHT, indexStep * 3));
        indices.append(getPlaneIndices(width, width, Direction::FRONT, indexStep * 4));
        indices.append(getPlaneIndices(width, width, Direction::BACK, indexStep * 5));

        return indices;
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
            0, 2, 1,
            2, 0, 3
        };
        return quadIndices;
    }

    // 3. Plane
    static QVector<Vertex> getPlaneVertices(int width, int height, Direction dir = Direction::FRONT) {
        QVector<Vertex> vertices;
        float halfWidth = static_cast<float>(width) * 0.5f;
        float halfHeight = static_cast<float>(height) * 0.5f;

        for (int i = 0; i <= width; i++) {
            for (int j = 0; j <= height; j++) {
                Vertex vertex;

                switch(dir) {
                    case Direction::UP:
                        vertex.position = {static_cast<float>(i) - halfWidth, halfHeight, static_cast<float>(j) - halfHeight};
                        vertex.normal = {0, 1, 0};
                        break;
                    case Direction::DOWN:
                        vertex.position = {static_cast<float>(i) - halfWidth, -halfHeight, static_cast<float>(j) - halfHeight};
                        vertex.normal = {0, -1, 0};
                        break;
                    case Direction::LEFT:
                        vertex.position = {-halfWidth, static_cast<float>(i) - halfHeight, static_cast<float>(j) - halfWidth};
                        vertex.normal = {-1, 0, 0};
                        break;
                    case Direction::RIGHT:
                        vertex.position = {halfWidth, static_cast<float>(i) - halfHeight, static_cast<float>(j) - halfWidth};
                        vertex.normal = {1, 0, 0};
                        break;
                    case Direction::FRONT:
                        vertex.position = {static_cast<float>(i) - halfWidth, static_cast<float>(j) - halfHeight, halfHeight};
                        vertex.normal = {0, 0, 1};
                        break;
                    case Direction::BACK:
                        vertex.position = {static_cast<float>(i) - halfWidth, static_cast<float>(j) - halfHeight, -halfHeight};
                        vertex.normal = {0, 0, -1};
                        break;
                }

                vertex.texCoord = {static_cast<float>(i) / float(width), static_cast<float>(j) / float(height)};
                vertices.append(vertex);
            }
        }

        return vertices;
    }
    static QVector<unsigned int> getPlaneIndices(int width, int height, Direction dir = Direction::FRONT, int startIndex = 0) {
        QVector<unsigned int> indices;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                int start = i * (height + 1) + j + startIndex;

                if (dir == Direction::FRONT || dir == Direction::RIGHT || dir == Direction::DOWN) {
                    indices << start << start + height + 2 << start + 1;
                    indices << start << start + height + 1 << start + height + 2;
                } else if (dir == Direction::BACK || dir == Direction::LEFT || dir == Direction::UP ){
                    indices << start << start + 1 << start + height + 2;
                    indices << start << start + height + 2 << start + height + 1;
                }
            }
        }

        return indices;
    }

    // TODO : 4. Capsule
    static const QVector<Vertex>& getCapsuleVertices(float radius, float height) {
        static const QVector<Vertex> capsuleVertices {};
        return capsuleVertices;
    }
    static const QVector<unsigned int>& getCapsuleIndices(float radius, float height) {
        static const QVector<unsigned int> capsuleIndices {};
        return capsuleIndices;
    }

    // 5. Sphere
    static QVector<Vertex> getSphereVertices(float radius, int resolution) {
        QVector<Vertex> vertices;
        vertices = getCubeVertices(resolution);
        for(auto &v : vertices) {
            QVector3D temp = v.position.normalized();
            v.position = temp * radius;
            v.normal = temp;
        }

        return vertices;
    }
    static QVector<unsigned int> getSphereIndices(float radius, int resolution) {
        QVector<unsigned int> indices = getCubeIndices(resolution);

        return indices;
    }

    ShapeData() = delete;

   private:

};


#endif  //SHAPEDATA_HPP
