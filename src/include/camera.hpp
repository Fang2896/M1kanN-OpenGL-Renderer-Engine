//
// Created by fangl on 2023/9/21.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <QVector3D>
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QOpenGLShader>

#include <QDebug>
#include <QtMath>


enum class CameraMove {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// parameters
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ZOOM = 45.0f;


class Camera
{
   public:
    explicit Camera(QVector3D position, GLfloat speed) {
        this->position = position;
        this->front = QVector3D(0.0f, 0.0f, -1.0f);
        this->worldUp = QVector3D(0.0f, 1.0f, 0.0f);
        this->yaw = YAW;
        this->pitch = PITCH;

        this->movementSpeed = speed;
        this->mouseSensitivity = SENSITIVITY;
        this->zoom = ZOOM;

        this->updateCameraData();
    }


    QMatrix4x4 getViewMatrix() const;
    void handleMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constraintPitch = true);
    void handleMouseScroll(GLfloat yOffset);
    void handleKeyboard(CameraMove direction, GLfloat deltaTime);

    QVector3D position;
    QVector3D worldUp;
    QVector3D front;

    QVector3D up;
    QVector3D right;

    //Euler Angles
    GLfloat pitch;
    GLfloat yaw;

    //Camera options
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

   private:
    void updateCameraData();

};


#endif  //CAMERA_HPP
