//
// Created by fangl on 2023/10/3.
//

#include "post_processing/post_process_screen.hpp"


PostProcessScreen::PostProcessScreen() : glFunc(nullptr), VBO(0) {}

PostProcessScreen::~PostProcessScreen() = default;

void PostProcessScreen::init() {
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if (!glFunc) {
        qFatal("Requires OpenGL >= 4.3");
    }
    glFunc->initializeOpenGLFunctions();

    float vertices[] = {
        // positions            // textures
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    glFunc->glGenBuffers(1, &VBO);
    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFunc->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glFunc->glGenVertexArrays(1, &VAO);
    glFunc->glBindVertexArray(VAO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                                  4 * sizeof(float), (void*)nullptr);

    glFunc->glEnableVertexAttribArray(1);
    glFunc->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                                  4 * sizeof(float), (void*)(2*sizeof(float)));

    glFunc->glBindVertexArray(0);
}

void PostProcessScreen::draw() {
    glFunc->glBindVertexArray(VAO);
    glFunc->glDrawArrays(GL_TRIANGLES, 0, 6);
    glFunc->glBindVertexArray(0);
}


