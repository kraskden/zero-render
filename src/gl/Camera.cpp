#include "Camera.h"

#include <cmath>
#include <QtGui/QMatrix3x3>
#include <QtGui/QMatrix4x4>

Camera::Camera(QVector3D eye, QVector3D center, float moveSpeed, float rotateSpeed, QObject *parent) :
    eye(eye), moveSpeed(moveSpeed), rotateSpeed(rotateSpeed),
    cameraFront(0, 0, -1), QObject(parent)
{
    startEye = eye;
    startFront = cameraFront;
    cameraUp = {0, 1, 0};
}

void Camera::forward() {
    zMove(-moveSpeed);
}

void Camera::backward() {
    zMove(moveSpeed);
}

void Camera::right() {
    sideMove(moveSpeed);
}

void Camera::left() {
    sideMove(-moveSpeed);
}

void Camera::zMove(float delta) {
    eye += cameraFront * -delta;
    //eye.setZ(eye.z() + delta);
    emit cameraChanged();
}

void Camera::sideMove(float delta) {
    eye += QVector3D::crossProduct(cameraFront, cameraUp).normalized() * delta;
    emit cameraChanged();
}

const QVector3D &Camera::getEye() const {
    return eye;
}

QVector3D Camera::getCenter() const {
    return eye + cameraFront;
}

const QVector3D &Camera::getCameraFront() const {
    return cameraFront;
}


float Camera::getMoveSpeed() const {
    return moveSpeed;
}

float Camera::getRotateSpeed() const {
    return rotateSpeed;
}

void Camera::rotateY(bool reverse) {
    int mul = reverse ? 1 : -1;
    rotate(QVector3D{0, 1, 0}, mul * rotateSpeed);
}

void Camera::rotateX(bool reverse) {
    int mul = reverse ? 1 : -1;
    QVector3D xRot = QVector3D::crossProduct(cameraUp, cameraFront).normalized();
    rotate(xRot, mul * rotateSpeed);

    QMatrix4x4 rotateMatrix = QMatrix4x4{};
    rotateMatrix.rotate(mul * rotateSpeed, cameraFront);
    cameraFront = rotateMatrix * cameraFront;

}

void Camera::rotate(const QVector3D &axis, float angle) {
    QMatrix4x4 rotateMatrix = QMatrix4x4{};
    rotateMatrix.rotate(angle, axis);
    cameraFront = rotateMatrix * cameraFront;
    emit cameraChanged();
}

void Camera::reset() {
    cameraFront = startFront;
    eye = startEye;
    emit cameraChanged();
}
