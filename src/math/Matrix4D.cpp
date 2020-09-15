#include "Matrix4D.h"

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
