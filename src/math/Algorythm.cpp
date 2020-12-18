#include "Algorythm.h"
#include <QImage>
#include <QtGui/QVector2D>
#include <QDebug>

QVector3D reflect(const QVector3D &v, const QVector3D &n) {
    // r = d − 2(d⋅n)n
    return v - QVector3D::dotProduct(v, n) * 2 * n;
}

Vec3i texel(const QImage &image, const QVector3D &pos) {
    float w = image.width();
    float h = image.height();

    float x = std::min(pos.x() * w, w - 1);
    float y = std::min(h * (1 - pos.y()), h - 1);
    if (x < 0 || y < 0) {
        //qDebug() << pos;
    }
    return QColor::fromRgb(image.pixel((int)x, (int)y));
}

QVector3D toBar(const Face &face, float x, float y) {
//    Vec3 n = new Vec3(v3.x - v1.x, v2.x - v1.x, v1.x - p.x)
//    .cross(new Vec3(v3.y - v1.y, v2.y - v1.y, v1.y - p.y));
    const QVector4D& v1 = face[0].screen;
    const QVector4D& v2 = face[1].screen;
    const QVector4D& v3 = face[2].screen;
    QVector3D n = QVector3D::crossProduct({v3.x() - v1.x(), v2.x() - v1.x(), v1.x() - x},
                                          {v3.y() - v1.y(), v2.y() - v1.y(), v1.y() - y});
    if (abs(n.z()) > 1e-3) {
        return QVector3D{1.f - (n.x() + n.y()) / n.z(), n.y() / n.z(), n.x() / n.z()};
    }
    return QVector3D{-1, -1, -1};
}
