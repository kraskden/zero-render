#ifndef GLPAINTER_CAMERA_H
#define GLPAINTER_CAMERA_H
#include <QtCore/QObject>
#include <QtGui/QVector3D>

class Camera : public QObject {
    Q_OBJECT

    QVector3D eye;
    QVector3D center;
    float speed;
    float angle;
    float radius;

public:
    Camera(QVector3D eye, QVector3D center, float speed, QObject* parent = nullptr);

    void incSpeed(double delta) {this->speed += delta; }
    void forward();
    void backward();
    void right();
    void left();

    void rotate(float delta);

    const QVector3D &getEye() const;

    const QVector3D &getCenter() const;

    float getAngle() const;

    float getRadius() const;

    float getSpeed() const;


signals:
    void cameraChanged(QVector3D eye, QVector3D center);

private:
    void zMove(float delta);
    void sideMove(float delta);

    static QVector3D cameraFront;
    static QVector3D cameraUp;
};


#endif //GLPAINTER_CAMERA_H
