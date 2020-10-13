#ifndef GLPAINTER_GLPAINTER_H
#define GLPAINTER_GLPAINTER_H


#include <QtGui/QPainter>
#include <QtCore/QMutex>
#include "../math/Vec3i.h"

class GlPainter {
    QPainter* painter;
    QAtomicInt* atomics;
    int* zBuffer;
    int width;

public:
    explicit GlPainter(QPainter *painter, QAtomicInt *atomics, int *zBuffer, int width);

    void clean(int width, int height);
    void setColor(const QColor &color);

    void asyncLine(int x1, int y1, int x2, int y2);
    void asyncTriangle(Vec3i t0, Vec3i t1, Vec3i t2, float intensity);

    QPainter* qPainter() {return painter;}

private:
    inline void lock(int lockIdx) {
        while(!atomics[lockIdx].testAndSetAcquire(0, 1)) {}
    }

    inline void unlock(int lockIdx) {
        atomics[lockIdx] = 0;
    }
};


#endif //GLPAINTER_GLPAINTER_H
