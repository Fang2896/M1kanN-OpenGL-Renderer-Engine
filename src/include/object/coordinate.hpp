//
// Created by fangl on 2023/9/28.
//

#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include "gl_configure.hpp"


class Coordinate {
   public:
    Coordinate();
    ~Coordinate();

    void initCoordinate();
    void drawCoordinate();

   private:
    GLFunctions_Core* glFunc = nullptr;

    GLuint coordVAO{};
    GLuint coordVBO{};

    const QVector<float> coordData = {
        // x
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        // y
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        // z
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f};
};

#endif  //COORDINATE_HPP
