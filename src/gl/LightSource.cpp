#include "LightSource.h"

void LightSource::updateVectors() {
    localRadius = radius * cosf(altitude);

    eye.setX(localRadius * sinf(azimuth));
    eye.setY(sinf(altitude) * radius);
    eye.setZ(localRadius * cosf(azimuth));

    front = (target - eye).normalized();
}
