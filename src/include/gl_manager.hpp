//
// Created by fangl on 2023/9/19.
//

#ifndef GL_MANAGER_HPP
#define GL_MANAGER_HPP



#include <QVector3D>
#include <QOpenGLWidget>
#include <QElapsedTimer>

#include "data_structures.hpp"
#include "gl_configure.hpp"

//#include "object/model.hpp"
//#include "object/shape.hpp"
#include "object/game_object.hpp"

#include "utils/camera.hpp"
#include "utils/resource_manager.hpp"

class GLManager : public QOpenGLWidget {
   public:
    explicit GLManager(QWidget* parent = nullptr, int width = 800,
                       int height = 400);
    ~GLManager() override;

   protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

   private:  // functions
    void initShaders();
    void initShaderValue();
    void initConfigureVariables();
    void initLightInfo();
    void initOpenGLSettings();

    // 单独绘制单个物体的函数，现在改为统一管理 -> Object基类
    void initCoordinate();
    void drawCoordinate();
    void drawObjects();

    void handleInput(GLfloat dt);
    void updateRenderData();
    static void checkGLVersion();

   private: // manager objects, shaders...
    QVector<std::shared_ptr<GameObject>> objectVec;

   private:  // key variables
    GLFunctions_Core* glFunc = nullptr;
    std::unique_ptr<Camera> m_camera;

    GLuint coordVAO;
    GLuint coordVBO;
    QVector<float> coordData;

    QElapsedTimer eTimer;

   private:  // configure variables
    GLboolean isLineMode;
    QVector3D backGroundColor;

    DirectLight directLight;

   private:  // control variables
    GLboolean keys[1024];
    GLboolean shiftDown;
    GLfloat defaultCameraMoveSpeed;

    GLboolean isFirstMouse;
    GLboolean isRightMousePress;
    GLint lastX;
    GLint lastY;

    GLfloat deltaTime;
    GLfloat lastFrame;

    QMatrix4x4 projection;
    QMatrix4x4 view;

   private: // for test
    // std::unique_ptr<Model> m_testModel;
    // std::unique_ptr<Shape> m_testCube;
};

#endif  //GL_MANAGER_HPP
