#ifndef GLPAINTER_GLPAINTER_H
#define GLPAINTER_GLPAINTER_H


#include <QtGui/QPainter>
#include <QtCore/QMutex>
#include "../math/Vec3i.h"
#include "../obj/ObjModel.h"
#include "../obj/Model3D.h"

class GlPainter {
    QImage* world;
    QAtomicInt* atomics;
    volatile int* zBuffer;
    BuffPoint* tBuffer;
    QRgb* screen;
    int width;
    int height;

public:
    explicit GlPainter(QImage* world, QAtomicInt *atomics, volatile int *zBuffer, BuffPoint *tBuffer, int width, int height);

    void fillTBuffer(Face* face, const Vec3i &t0, const Vec3i &t1, const Vec3i &t2);

    void putLightPoint(const Model3D *model, const Face &face, int px, const QVector3D &bar,
                       const QVector3D &inverseLight, const QVector3D &viewFront);

private:
    inline void lock(int lockIdx) {
        while(!atomics[lockIdx].testAndSetOrdered(0, 1)) {}
    }

    inline void unlock(int lockIdx) {
        atomics[lockIdx] = 0;
    }
};


#endif //GLPAINTER_GLPAINTER_H
