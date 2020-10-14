#include "Matrix4D.h"

#include <cmath>
#include "../common/const.h"

QVector3D nullVector = QVector3D{};

QMatrix4x4 matrix::scale_move(const QVector3D &scale, const QVector3D &transposition) {
    float data[] = {
            scale.x(), 0, 0, transposition.x(),
            0, scale.y(), 0, transposition.y(),
            0, 0, scale.z(), transposition.z(),
            0, 0, 0, 1
    };
    return QMatrix4x4{data};
}

QMatrix4x4 matrix::scale(const QVector3D &scale) {
    return scale_move(scale, nullVector);
}

QMatrix4x4 matrix::move(const QVector3D &transposition) {
    return scale_move(nullVector, transposition);
}

QMatrix4x4 matrix::identify() {
    return scale_move(nullVector, nullVector);
}

QMatrix4x4 matrix::view(const QVector3D &eye, const QVector3D &target, const QVector3D &up) {
    QVector3D z = (eye - target).normalized();
    QVector3D x = (QVector3D::crossProduct(up, z)).normalized();
    QVector3D y = (QVector3D::crossProduct(z, x)).normalized();

    float data[] = {
            x.x(), x.y(), x.z(), -QVector3D::dotProduct(x, eye),
            y.x(), y.y(), y.z(), -QVector3D::dotProduct(y, eye),
            z.x(), z.y(), z.z(), -QVector3D::dotProduct(z, eye),
            0, 0, 0, 1
    };
    return QMatrix4x4{data};
}

QMatrix4x4 matrix::projection_rel(float aspect, float fov, float z_near, float z_far) {
    float x11 = 1 / (aspect * tanf(fov / 2));
    float x22 = 1 / tanf(fov / 2);
    float x33 = z_far / (z_near - z_far);
    float x34 = z_near * z_far / (z_near - z_far);
    float data[] = {
            x11, 0, 0, 0,
            0, x22, 0, 0,
            0, 0, x33, x34,
            0, 0, -1, 0
    };
    return QMatrix4x4{data};
}

QMatrix4x4 matrix::projection_abs(float width, float height, float z_near, float z_far) {
    float x11 = 2 * z_near / width;
    float x22 = 2 * z_near / height;
    float x33 = z_far / (z_near - z_far);
    float x34 = z_near * z_far / (z_near - z_far);

    float data[] = {
            x11, 0, 0, 0,
            0, x22, 0, 0,
            0, 0, x33, x34,
            0, 0, -1, 0
    };
    return QMatrix4x4{data};
}

QMatrix4x4 matrix::viewport(float width, float height) {
    float data[] = {
            width / 2 , 0, 0, width / 2,
            0, -height / 2, 0, height / 2,
            0, 0, DEPTH / 2, DEPTH / 2, // 1
            0, 0, 0, 1
    };
    return QMatrix4x4{data};
}

QMatrix4x4 matrix::projection_my(float distance) {
    float data[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, -1.f/distance, 1
    };
    return QMatrix4x4{data};
}

