#include "Algorythm.h"
#include <QImage>

QVector3D toBarycentric(const Face &face, float x, float y) {
    const QVector4D& a = face[0].screen;
    const QVector4D& b = face[1].screen;
    const QVector4D& c = face[2].screen;

    float beta_top = (a.y() - c.y()) * x + (c.x() - a.x()) * y + a.x() * c.y() - c.x() * a.y();
    float beta_bottom = (a.y() - c.y()) * b.x() + (c.x() - a.x()) * b.y() + a.x() * c.y() - c.x() * a.y();

    float gamma_top = (a.y() - b.y()) * x + (b.x() - a.x()) * y + a.x() * b.y() - b.x() * a.y();
    float gamma_bottom = (a.y() - b.y()) * c.x() + (b.x() - a.x()) * c.y() + a.x() * b.y() - b.x() * a.y();

    float beta = beta_top / beta_bottom;
    float gamma = gamma_top / gamma_bottom;
    float alpha = 1 - beta - gamma;

    return QVector3D{alpha, beta, gamma};
}

QVector3D reflect(const QVector3D &v, const QVector3D &n) {
    // r = d − 2(d⋅n)n
    return v - QVector3D::dotProduct(v, n) * 2 * n;
}

Vec3i texel(const QImage *image, const QVector3D &pos, const Vec3i &def) {
    if (isnanf(pos.x()) || isnanf(pos.y())) {
        return def;
    }
//    printf("%f %f\n", pos.x(), pos.y());
    if (!image || image->isNull()) {
        return def;
    }
    //return def;

    float x = abs(pos.x() * image->width());
    float y = abs(pos.y() * image->height());
    int pixelIdx = y * image->width() + x + 0.5;
    QRgb* data = (QRgb*)image->constBits();
    auto* pixel = (uint8_t*)(data + pixelIdx);
//    return {pixel[1], pixel[2], pixel[3]};

    return QColor::fromRgb(data[pixelIdx]);
}
