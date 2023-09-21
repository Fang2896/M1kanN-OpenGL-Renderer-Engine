//
// Created by fangl on 2023/9/21.
//

#include "camera.hpp"


QMatrix4x4 Camera::getViewMatrix() const
{
    QMatrix4x4 view;
    view.lookAt(this->position, this->position + this->front, this->up);
    return view;
}

void Camera::handleKeyboard(CAMERA_MOVE direction, GLfloat deltaTime)
{
    GLfloat velocity = this->movementSpeed * deltaTime;
    if (direction == CAMERA_MOVE::FORWARD)
        this->position += this->front * velocity;
    if (direction == CAMERA_MOVE::BACKWARD)
        this->position -= this->front * velocity;
    if (direction == CAMERA_MOVE::LEFT)
        this->position -= this->right * velocity;
    if (direction == CAMERA_MOVE::RIGHT)
        this->position += this->right * velocity;
    if (direction == CAMERA_MOVE::UP)
        this->position += this->worldUp * velocity;
    if (direction == CAMERA_MOVE::DOWN)
        this->position -= this->worldUp * velocity;
}

void Camera::handleMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constraintPitch)
{
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw += xOffset;
    this->pitch += yOffset;

    if (constraintPitch) {
        if (this->pitch > 89.0f)
            this->pitch = 89.0f;
        if (this->pitch < -89.0f)
            this->pitch = -89.0f;
    }

    this->updateCameraData();
}

void Camera::handleMouseScroll(GLfloat yOffset)
{
    if (this->zoom >= 1.0f && this->zoom <= 45.0f)
        this->zoom -= yOffset;
    if (this->zoom > 45.0f)
        this->zoom = 45.0f;
    if (this->zoom < 1.0f)
        this->zoom = 1.0f;
}

void Camera::updateCameraData()
{
    GLfloat yawRadian = qDegreesToRadians(this->yaw);
    GLfloat pitchRadian = qDegreesToRadians(this->pitch);

    QVector3D front3(cos(yawRadian) * cos(pitchRadian),
                     sin(pitchRadian),
                     sin(yawRadian) * cos(pitchRadian));

    this->front = front3.normalized();
    this->right = QVector3D::crossProduct(this->front, this->worldUp).normalized();
    this->up = QVector3D::crossProduct(this->right, this->front).normalized();
}
