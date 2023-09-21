//
// Created by fangl on 2023/9/19.
//

#ifndef GL_MANAGER_HPP
#define GL_MANAGER_HPP

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include "camera.hpp"

const QVector3D CAMERA_POSITION(0.0f, 0.1f, 3.0f);


class GLManager : public QOpenGLWidget {
   public:
    explicit GLManager(QWidget *parent = nullptr,
                       int width = 800, int height = 400);
    ~GLManager() override;

   protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

   private: // functions
    void checkGLVersion();

   private: // variables
    QOpenGLFunctions *functions = nullptr;

    std::unique_ptr<Camera> m_camera;

};

#endif  //GL_MANAGER_HPP
