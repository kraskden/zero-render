#include "Camera.h"

#include <cmath>

void Camera::move(CameraMovement movement, MoveDirection direction) {
    float mul = direction == MoveDirection::FORWARD ? 1 : -1;
    switch (movement) {
        case Z:
            this->eye += front * mul * movementSpeed;
            break;
        case STRAFE:
            this->eye += right * mul * movementSpeed;
            break;
        case PITCH:
            this->changePitch(mul);
            break;
        case YAW:
            this->changeYaw(mul);
            break;
    }
}

void Camera::updateCameraVectors() {
    front.setX(cosf(yaw) * cosf(pitch));
    front.setY(sinf(pitch));
    front.setZ(sinf(yaw) * cosf(pitch));
    front.normalize();

    right = QVector3D::crossProduct(front, worldUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}
