#include "GlPainter.h"

#include "math.h"

#include "Scene.h"

void GlPainter::clean(int width, int height) {
    painter->fillRect(0, 0, width, height, QColorConstants::Black);
}

void GlPainter::line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2-y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        painter->drawPoint(x1, y1);
        if (x1 == x2 && y1 == y2) {
            return;
        }
        int err2 = err * 2;
        if (err2 > dy) {
            err += dy;
            x1 += sx;
        }
        if (err2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}

GlPainter::GlPainter(QPainter *painter) : painter(painter) {}

void GlPainter::setColor(const QColor &color) {
    painter->setPen(color);
}

void GlPainter::line(const QPoint &start, const QPoint &end) {
    line(start.x(), start.y(), end.x(), end.y());
}

void GlPainter::asyncLine(int x1, int y1, int x2, int y2, QMutex* mutexes, int height) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2-y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        int lockIdx = y1 * height + x1;
        mutexes[lockIdx].lock();
        painter->drawPoint(x1, y1);
        mutexes[lockIdx].unlock();
        if (x1 == x2 && y1 == y2) {
            return;
        }
        int err2 = err * 2;
        if (err2 > dy) {
            err += dy;
            x1 += sx;
        }
        if (err2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
}


