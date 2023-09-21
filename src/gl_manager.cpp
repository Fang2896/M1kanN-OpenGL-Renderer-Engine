//
// Created by fangl on 2023/9/19.
//

#include "gl_manager.hpp"


GLManager::GLManager(QWidget *parent, int width, int height) : QOpenGLWidget(parent) {
    this->setGeometry(10, 20, width, height);
}

GLManager::~GLManager() {

}

void GLManager::initializeGL() {
    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    m_camera = std::make_unique<Camera>(CAMERA_POSITION);

    core->glEnable(GL_DEPTH_TEST);

    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    core->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GLManager::resizeGL(int w, int h) {
    core->glViewport(0, 0, w, h);
}

void GLManager::paintGL() {

}

