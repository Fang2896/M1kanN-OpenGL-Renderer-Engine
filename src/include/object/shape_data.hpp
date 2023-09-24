//
// Created by fangl on 2023/9/24.
//

#ifndef SHAPEDATA_HPP
#define SHAPEDATA_HPP

#include <QVector>

class ShapeData {
   public:
    static QVector<float> getCoordinateVertices() {
        static QVector<float> coordinateVertices = {
            // x
            0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            // y
            0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            // z
            0.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f};
        return coordinateVertices;
    }

    // basic geometry
    // 1. Cube
    static const QVector<float>& getCubeVertices() {
        static QVector<float> cubeVertices = {
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

    // 2. Plane
    static const QVector<float>& getPlaneVertices() {
        static QVector<float> planeVertices =
            {
                // positions       // normals         // textures
                -0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
                0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            };
        return planeVertices;
    }
    static const QVector<unsigned int>& getPlaneIndices() {
        static QVector<unsigned int> planeIndices = {
            0, 1, 2,
            2, 3, 0
        };
        return planeIndices;
    }

    ShapeData() = delete;

   private:

};


#endif  //SHAPEDATA_HPP
