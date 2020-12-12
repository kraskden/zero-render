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
    return QColor::fromRgb(image.pixel((int)x, (int)y));
}


QVector3D toBarycentric2D(const Face &face, float x, float y) {
    const QVector4D& a = face[0].screen;
    const QVector4D& b = face[1].screen;
    const QVector4D& c = face[2].screen;

    float reverseDet = 1.f / ((b.y() - c.y()) * (a.x() - c.x())
                            + (c.x() - b.x()) * (a.y() - c.y()));

    if (isnanf(reverseDet)) {
        return {-1, -1, -1};
    }

    float alpha = reverseDet * ((b.y() - c.y()) * (x - c.x()) +
                                (c.x() - b.x()) * (y - c.y()));
    float beta = reverseDet * ((c.y() - a.y()) * (x - c.x()) +
                               (a.x() - c.x()) * (y - c.y()));
    float gamma = 1 - alpha - beta;

    return QVector3D{alpha, beta, gamma};
}