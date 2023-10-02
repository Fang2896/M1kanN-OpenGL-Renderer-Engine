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

#include "object/coordinate.hpp"
#include "object/game_object.hpp"

#include "utils/camera.hpp"
#include "utils/resource_manager.hpp"


class GLManager : public QOpenGLWidget {
   public:
    explicit GLManager(QWidget* parent = nullptr, int width = 800,
                       int height = 400);
    ~GLManager() override;

   public:  // api for MainWindow
    void clearObjects();
    int addObject(const QString& mPath = "");
    int addObject(ObjectType objType, float width = 0.0f, float height = 0.0f);

    void deleteObject(GLuint id);
    [[nodiscard]] const std::map<GLuint, std::shared_ptr<GameObject>>& getAllGameObjectMap() const;
    std::shared_ptr<GameObject> getTargetGameObject(GLuint id);

    // configure setter
    void setEnableLighting(GLboolean enableLighting);
    void setLineMode(GLboolean enableLineMode);
    void setDepthMode(GLboolean depMode);
    void setCullMode(CullModeType type);

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

   private: // control functions...
    void handleInput(GLfloat dt);
    void updateRenderData();
    static void checkGLVersion();

   private:  // functions
    void initShaders();
    void initShaderValue();
    void initConfigureVariables();
    void initLightInfo();
    void initOpenGLSettings();

   private: // object manager functions
    void drawObjects();

   private: // objects member variables
    const QString modelDirectory = "../assets/models";
    std::map<GLuint, std::shared_ptr<GameObject>> objectMap;

   private:  // key variables
    GLFunctions_Core* glFunc = nullptr;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Coordinate> coordinate;

    QElapsedTimer eTimer;

   private:  // configure variables
    GLboolean isLineMode;
    GLboolean isLighting;
    GLboolean depthMode;
    QVector3D backGroundColor;
    CullModeType cullType;

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
