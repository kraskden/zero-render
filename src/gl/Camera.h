#ifndef GLPAINTER_CAMERA_H
#define GLPAINTER_CAMERA_H

#include <QtCore/QObject>
#include <QtGui/QVector3D>

#include "../math/Matrix4D.h"

#include <cmath>

enum CamMov {
    Z, STRAFE, PITCH, YAW
};

enum CamSide {
    FORWARD = 1,
    BACKWARD = -1
};

class Camera : public QObject {
    Q_OBJECT

    QVector3D up;
    QVector3D front;
    QVector3D eye;
    QVector3D worldUp;
    QVector3D right;

    float yaw;
    float pitch;

    float movementSpeed;
    float rotateSpeed;

public:
    Camera(const QVector3D &eye, float yaw, float pitch, float movementSpeed, float rotateSpeed, QObject* parent = nullptr)
            : QObject(parent), eye(eye), yaw(yaw), pitch(pitch), movementSpeed(movementSpeed),
              rotateSpeed(rotateSpeed) {
        up = QVector3D{0, 1, 0};
        worldUp = QVector3D{0, 1, 0};
        updateCameraVectors();
    }

public:
    void move(CamMov movement, CamSide direction);

    void incMovementSpeed(float delta) {movementSpeed += delta; }
    void incRotateSpeed(float delta) {rotateSpeed += delta;}

    QMatrix4x4 getViewMatrix() {
        return matrix::view(eye, eye + front, up);
    }

    const QVector3D &getFront() const {
        return front;
    }

    const QVector3D &getEye() const {
        return eye;
    }

    float getYaw() const {
        return yaw;
    }

    float getPitch() const {
        return pitch;
    }

    float getMovementSpeed() const {
        return movementSpeed;
    }

    float getRotateSpeed() const {
        return rotateSpeed;
    }

    void reset(const QVector3D& eye, float yaw, float pitch) {
        this->eye = eye;
        this->yaw = yaw;
        this->pitch = pitch;
        this->up = QVector3D{0, 1, 0};
        updateCameraVectors();
    }

private:
    void changePitch(float sign) {
        float limit = M_PI / 2 - 0.05;
        pitch += rotateSpeed * sign;
        pitch = pitch > limit ? limit : pitch;
        pitch = pitch < -limit ? -limit : pitch;
        updateCameraVectors();
    }

    void changeYaw(float sign) {
        yaw += rotateSpeed * sign;
        updateCameraVectors();
    }

    void updateCameraVectors();

};


#endif //GLPAINTER_CAMERA_H
