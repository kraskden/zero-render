#ifndef GLPAINTER_MATRIX4D_H
#define GLPAINTER_MATRIX4D_H

#include <QtGui/QMatrix4x4>

namespace matrix {
    QMatrix4x4 scale_move(const QVector3D& scale, const QVector3D& transposition);
    QMatrix4x4 scale(const QVector3D& scale);
    QMatrix4x4 move(const QVector3D& transposition);
    QMatrix4x4 identify();

    QMatrix4x4 view(const QVector3D& eye, const QVector3D& target, const QVector3D& up);
    QMatrix4x4 projection_rel(float aspect, float fov, float z_near, float z_far);
    QMatrix4x4 projection_abs(float width, float  height, float z_near, float z_far);
    QMatrix4x4 projection_my(float distance);
    QMatrix4x4 viewport(float width, float height);
}

#endif //GLPAINTER_MATRIX4D_H
