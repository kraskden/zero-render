#ifndef GLPAINTER_MATRIX4D_H
#define GLPAINTER_MATRIX4D_H

#include <QtGui/QMatrix4x4>

namespace matrix {
    QMatrix4x4 scale_move(const QVector3D& scale, const QVector3D& transposition);
    QMatrix4x4 scale(const QVector3D& scale);
    QMatrix4x4 move(const QVector3D& transposition);
    QMatrix4x4 identify();
}

#endif //GLPAINTER_MATRIX4D_H
