#ifndef GLPAINTER_LIGHTSOURCE_H
#define GLPAINTER_LIGHTSOURCE_H

#include <QtCore/QObject>
#include <QtGui/QVector3D>
#include <math.h>

class LightSource : public QObject {
    Q_OBJECT

    float radius;
    float localRadius;

    QVector3D front;
    QVector3D eye;
    QVector3D target;

    float altitude;
    float azimuth;

    float speed;

public:
    LightSource(QVector3D target, float radius, float altitude, float azimuth, float speed, QObject* parent) :
        QObject(parent), target(target), radius(radius), altitude(altitude), azimuth(azimuth), speed(speed)
    {
        updateVectors();
    }

    const QVector3D &getFront() const {
        return front;
    }

    float getAltitude() const {
        return altitude;
    }

    float getAzimuth() const {
        return azimuth;
    }

    void changeAzimuth(float sign) {
        azimuth += sign * speed;
        updateVectors();
    }

    void reset(float radius, float altitude, float azimuth) {
        this->radius = radius;
        this->altitude = altitude;
        this->azimuth = azimuth;
        updateVectors();
    }

    void changeAltitude(float sign) {
        float limit = M_PI_2 - 0.05;
        altitude += speed * sign;
        altitude = std::max(-limit, altitude);
        altitude = std::min(limit, altitude);
        updateVectors();
    }

private:
    void updateVectors();
};


#endif //GLPAINTER_LIGHTSOURCE_H
