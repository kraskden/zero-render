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

public:
    explicit GlPainter(QImage* world, QAtomicInt *atomics, volatile int *zBuffer, BuffPoint *tBuffer, int width);

    void asyncLine(int x1, int y1, int x2, int y2, QRgb color);
    void asyncTriangle(Vec3i t0, Vec3i t1, Vec3i t2, float intensity);

    void fillTBuffer(Face* face, Vec3i t0, Vec3i t1, Vec3i t2);
    void putPoint(int idx);

    void putLightPoint(const Model3D *model, const Face &face, int px, float x, float y,
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
