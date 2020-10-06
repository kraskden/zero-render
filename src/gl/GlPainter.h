#ifndef GLPAINTER_GLPAINTER_H
#define GLPAINTER_GLPAINTER_H


#include <QtGui/QPainter>
#include <QtCore/QMutex>

class Scene;

class GlPainter {
    QPainter* painter;
    QAtomicInt* atomics;
    int height;

public:
    explicit GlPainter(QPainter *painter, QAtomicInt* atomics, int height);

    void clean(int width, int height);
    void setColor(const QColor &color);

    void asyncLine(int x1, int y1, int x2, int y2);

    QPainter* qPainter() {return painter;}
};


#endif //GLPAINTER_GLPAINTER_H
