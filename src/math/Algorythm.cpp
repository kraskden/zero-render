#include "Algorythm.h"
#include <QImage>
#include <QtGui/QVector2D>
#include <QDebug>

QVector3D toBarycentric2(const Face& face, float x, float y)
{
    QVector2D a = face[0].screen.toVector2D();
    QVector2D b = face[1].screen.toVector2D();
    QVector2D c = face[2].screen.toVector2D();

    QVector2D v0 = b - a;
    QVector2D v1 = c - a;
    QVector2D v2 = QVector2D{x, y} - a;

    float reverse_det = 1 / (v0.x() * v1.y() - v1.x() * v0.y());
    float v = (v2.x() * v1.y() - v1.x() * v2.y()) * reverse_det;
    float w = (v0.x() * v2.y() - v2.x() * v0.y()) * reverse_det;
    float u = 1.0f - v - w;
    return {u, v, w};
}

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

Vec3i texel(const QImage &image, const QVector3D &pos) {
    float w = image.width();
    float h = image.height();

    if (isnanf(pos.x()) || isnanf(pos.y())) {
        qDebug() << "Pos is nan";
        //throw std::runtime_error("Pos is NaN");
        return QColorConstants::Red;
   }

//    float tX = pos.x() * w;
//    float tY = h - pos.y() * h;
//    if (tX < 0 || tY < 0 || tX > w - 1 || tY > h - 1) {
//        return QColorConstants::Cyan;
//    }

    float x = std::max(std::min(w - 1, pos.x() * w), 0.f);
    float y = std::max(std::min(h - 1, h - pos.y() * h), 0.f);
    // TODO: use const data
    return QColor::fromRgb(image.pixel((int)x, (int)y));
}

QVector3D toBarycentric3(const Face &face, float x, float y) {
    QVector2D a = face[0].screen.toVector2D();
    QVector2D b = face[1].screen.toVector2D();
    QVector2D c = face[2].screen.toVector2D();

    float reverseDet = 1 / ((b.y() - c.y()) * (a.x() - c.x())
            + (c.x() - b.x()) * (a.y() - c.y()));

    float alpha = reverseDet * ((b.y() - c.y()) * (x - c.x()) +
            (c.x() - b.x()) * (y - c.y()));
    float beta = reverseDet * ((c.y() - a.y()) * (x - c.x()) +
            (a.x() - c.x()) * (y - c.y()));
    float gamma = 1 - alpha - beta;
    return QVector3D{alpha, beta, gamma};
}

bool isNorm(float x) {
    return x > 0.f && x < 1.f;
}

QVector3D toBarycentric2D(const Face &face, float x, float y) {
    // TODO: vec4d is bullshit
    const QVector4D& a = face[0].screen;
    const QVector4D& b = face[1].screen;
    const QVector4D& c = face[2].screen;

    float reverseDet = 1.f / ((b.y() - c.y()) * (a.x() - c.x())
                            + (c.x() - b.x()) * (a.y() - c.y()));

    float alpha = reverseDet * ((b.y() - c.y()) * (x - c.x()) +
                                (c.x() - b.x()) * (y - c.y()));
    float beta = reverseDet * ((c.y() - a.y()) * (x - c.x()) +
                               (a.x() - c.x()) * (y - c.y()));
    float gamma = 1 - alpha - beta;

    if (isnanf(alpha) || isnanf(beta) || isnanf(gamma)) {
        // qDebug() << "! " << alpha << beta << gamma << reverseDet;
    }

    if (!isNorm(alpha) || !isNorm(beta) || !isNorm(gamma)) {
//        qDebug() << QVector3D{alpha, beta, gamma} << face[0].screen << face[1].screen << face[2].screen << "::" <<
//            x << y;
    }

    return QVector3D{alpha, beta, gamma};
}

QVector3D toBarycentric2D_int(const Vec3i &a, const Vec3i &b, const Vec3i &c, float x, float y) {

    float reverseDet = 1.f / ((b.y - c.y) * (a.x - c.x)
                              + (c.x - b.x) * (a.y - c.y));

    float alpha = reverseDet * ((b.y - c.y) * (x - c.x) +
                                (c.x - b.x) * (y - c.y));
    float beta = reverseDet * ((c.y - a.y) * (x - c.x) +
                               (a.x - c.x) * (y - c.y));
    float gamma = 1 - alpha - beta;

    if (isnanf(alpha) || isnanf(beta) || isnanf(gamma)) {
         qDebug() << "! " << alpha << beta << gamma << reverseDet;
    }

    if (!isNorm(alpha) || !isNorm(beta) || !isNorm(gamma)) {

//        qDebug() << QVector3D{alpha, beta, gamma} << a.toVector3D() << b.toVector3D() << c.toVector3D() << "::" <<
//                 x << y;
    }

    return QVector3D{alpha, beta, gamma};
}
