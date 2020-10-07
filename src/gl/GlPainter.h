#ifndef GLPAINTER_GLPAINTER_H
#define GLPAINTER_GLPAINTER_H


#include <QtGui/QPainter>
#include <QtCore/QMutex>

class GlPainter {
    QPainter* painter;
    QAtomicInt* atomics;
    int height;

public:
    explicit GlPainter(QPainter *painter, QAtomicInt* atomics, int height);

    void clean(int width, int height);
    void setColor(const QColor &color);

    void asyncLine(int x1, int y1, int x2, int y2);
    void asyncTriangle(QPoint p1, QPoint p2, QPoint p3);

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
