//
// Created by fangl on 2023/9/19.
//

#ifndef GL_MANAGER_HPP
#define GL_MANAGER_HPP

#include <QElapsedTimer>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>

#include "camera.hpp"
#include "shape.hpp"
#include "resource_manager.hpp"


class GLManager : public QOpenGLWidget {
   public:
    explicit GLManager(QWidget* parent = nullptr, int width = 800,
                       int height = 400);
    ~GLManager() override;

    void handleKeyPressEvent(QKeyEvent *event);
    void handleKeyReleaseEvent(QKeyEvent *event);

   protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

   private:  // functions
    void initObjects();
    void initResources();
    void drawCoordinate();

    void handleInput(GLfloat dt);
    void updateRenderData();

    static void checkGLVersion();
    void initConfigureVariables();

   private:  // key variables
    QOpenGLFunctions_4_3_Core* glFunc = nullptr;
    std::unique_ptr<Camera> m_camera;

    GLuint coordVAO;
    GLuint coordVBO;
    QVector<float> coordData;

    QElapsedTimer eTimer;

   private:  // configure variables
    GLboolean isLineMode;

   private:  // control variables
    GLboolean keys[1024];

    GLboolean isFirstMouse;
    GLboolean isRightMousePress;
    GLint lastX;
    GLint lastY;

    GLfloat deltaTime;
    GLfloat lastFrame;

    QMatrix4x4 projection;
    QMatrix4x4 view;
};

#endif  //GL_MANAGER_HPP
