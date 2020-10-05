#ifndef GLPAINTER_CAMERA_H
#define GLPAINTER_CAMERA_H
#include <QtCore/QObject>
#include <QtGui/QVector3D>


class Camera : public QObject {
    Q_OBJECT

    QVector3D startEye;
    QVector3D startFront;

    QVector3D eye;
    QVector3D cameraFront;
    QVector3D cameraUp;

    float moveSpeed;
    float rotateSpeed;

    float pitch = 0;
    float yaw = 0;

public:
    Camera(QVector3D eye, QVector3D center, float moveSpeed, float rotateSpeed, QObject* parent = nullptr);

    void incMoveSpeed(float delta) { this->moveSpeed += delta; }
    void incRotateSpeed(float delta) {this->rotateSpeed += delta; }

    // zMove, xMove, YMove
    void forward();
    void backward();
    void right();
    void left();


    void rotateY(bool reverse);
    void rotateX(bool reverse);

    void reset();

    const QVector3D &getEye() const;

    QVector3D getCenter() const;

    const QVector3D &getCameraFront() const;

    float getMoveSpeed() const;

    float getRotateSpeed() const;


signals:
    void cameraChanged();

private:
    void zMove(float delta);
    void sideMove(float delta);
    void rotate(const QVector3D& axis, float angle);
};


#endif //GLPAINTER_CAMERA_H
