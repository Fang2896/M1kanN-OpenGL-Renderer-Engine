//
// Created by fangl on 2023/9/28.
//

#include "coordinate.hpp"


Coordinate::Coordinate() {
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if (!glFunc) {
        qFatal("Requires OpenGL >= 4.1");
    }
}

void Coordinate::initCoordinate() {
    glFunc->glGenBuffers(1, &coordVBO);
    glFunc->glBindBuffer(GL_ARRAY_BUFFER, coordVBO);
    glFunc->glBufferData(GL_ARRAY_BUFFER, coordData.size() * sizeof(float),
                         coordData.data(), GL_STATIC_DRAW);

    glFunc->glGenVertexArrays(1, &coordVAO);
    glFunc->glBindVertexArray(coordVAO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, coordVBO);

    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  (GLsizei)3 * sizeof(float), (void*)nullptr);
    glFunc->glBindVertexArray(0);
}

void Coordinate::drawCoordinate() {
    glFunc->glLineWidth(10.0f);
    glFunc->glBindVertexArray(coordVAO);
    glFunc->glDrawArrays(GL_LINES, 0, (GLsizei)coordData.size() / 3);
    glFunc->glBindVertexArray(0);
    glFunc->glLineWidth(1.0f);
}



Coordinate::~Coordinate() = default;
