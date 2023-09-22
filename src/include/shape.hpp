//
// Created by fangl on 2023/9/22.
//

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <QDebug>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>
#include <QVector>

enum class DataType { kOnlyPosition, kPosTexNor };

enum class DrawMode {
    POINTS,
    LINES,
    LINE_STRIP,
    LINE_LOOP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

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
};

#endif  //SHAPE_HPP
