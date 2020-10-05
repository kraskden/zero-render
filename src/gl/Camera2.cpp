#include "Camera2.h"

#include <cmath>

void Camera2::move(CameraMovement movement, MoveDirection direction) {
    float mul = direction == MoveDirection::FORWARD ? 1 : -1;
    switch (movement) {
        case Z:
            this->eye += front * mul * movementSpeed;
        case STRAFE:
            this->eye += right * mul * movementSpeed;
        case PITCH:
            this->changePitch(mul);
        case YAW:
            this->changeYaw(mul);
    }
}

void Camera2::updateCameraVectors() {
    front.setX(cosf(yaw) * cosf(pitch));
    front.setY(sinf(pitch));
    front.setZ(sinf(yaw) * cosf(pitch));
    front.normalize();

    right = QVector3D::crossProduct(front, worldUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}
