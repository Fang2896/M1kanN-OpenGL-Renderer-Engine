//
// Created by fangl on 2023/9/19.
//

#include "gl_manager.hpp"


GLManager::GLManager(QWidget *parent, int width, int height) : QOpenGLWidget(parent) {
    this->setGeometry(10, 20, width, height);
}

GLManager::~GLManager() = default;

void GLManager::initializeGL() {
    checkGLVersion();

    functions = QOpenGLContext::currentContext()->functions();
    m_camera = std::make_unique<Camera>(CAMERA_POSITION);

    functions->glEnable(GL_DEPTH_TEST);

    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    functions->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);



}

void GLManager::resizeGL(int w, int h) {
    functions->glViewport(0, 0, w, h);
}

void GLManager::paintGL() {

}







void GLManager::checkGLVersion() {
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (context) {
        QSurfaceFormat format = context->format();
        int majorVersion = format.majorVersion();
        int minorVersion = format.minorVersion();
        qDebug() << "OpenGL Version:" << majorVersion << "." << minorVersion;
    } else {
        qDebug() << "OpenGL Context Empty.";
    }
}


