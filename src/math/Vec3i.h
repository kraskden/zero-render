#ifndef GLPAINTER_VEC3I_H
#define GLPAINTER_VEC3I_H


#include <QtGui/QVector3D>
#include <QColor>
#include <cmath>

struct Vec3i {
    int x; int y; int z;

    Vec3i() {}
    Vec3i(int x, int y, int z);
    Vec3i(const QVector3D& v) {
//        x = v.x();
//        y = v.y();
//        z = v.z();

        x = v.x() + .5;
        y = v.y() + .5;
        z = v.z() + .5;
    }

    Vec3i(const QColor& color) {
        x = color.red();
        y = color.green();
        z = color.blue();
    }

    Vec3i operator+(const Vec3i& v) const {
        return {x + v.x, y + v.y, z + v.z};
    }

    Vec3i operator-(const Vec3i& v) const {
        return {x - v.x, y - v.y, z - v.z};
    }

    friend Vec3i operator*(const Vec3i& v, int f);
    friend QVector3D operator*(const Vec3i& v, float f);
    friend QVector3D operator/(const Vec3i& v, float f);

    QVector3D toVector3D() const {
        return {(float)x, (float)y, (float)z};
    }

    QRgb toRgb() const;
};


#endif //GLPAINTER_VEC3I_H
