#include "Camera.h"

#include <cmath>

QVector3D Camera::cameraFront = QVector3D{0, 0, -1};
QVector3D Camera::cameraUp = QVector3D{0, 1, 0};

Camera::Camera(QVector3D eye, QVector3D center, float speed, QObject *parent) :
    eye(eye), center(center), speed(speed), QObject(parent), angle(0), radius(eye.z()) {

}

void Camera::forward() {
    zMove(-speed);
}

void Camera::backward() {
    zMove(speed);
}

void Camera::right() {
    sideMove(speed);
}

void Camera::left() {
    sideMove(-speed);
}

void Camera::zMove(float delta) {
//    eye.setZ(eye.z() + delta);
    radius += delta;
    rotate(0);
    //emit cameraChanged(eye, center);
}

void Camera::sideMove(float delta) {
    eye += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * delta;
    center = eye + cameraFront;
    emit cameraChanged(eye, center);
}

const QVector3D &Camera::getEye() const {
    return eye;
}

const QVector3D &Camera::getCenter() const {
    return center;
}

float Camera::getSpeed() const {
    return speed;
}

void Camera::rotate(float delta) {
    angle += delta;
    eye.setX(radius * sinf(angle));
    eye.setZ(radius * cosf(angle));
    emit cameraChanged(eye, center);
}

float Camera::getAngle() const {
    return angle;
}

float Camera::getRadius() const {
    return radius;
}


