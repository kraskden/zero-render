#include "GlPainter.h"

#include <cmath>

#include "Scene.h"

void GlPainter::clean(int width, int height) {
    painter->fillRect(0, 0, width, height, QColorConstants::Black);
}


GlPainter::GlPainter(QPainter *painter, QAtomicInt *atomics, int height) : painter(painter), atomics(atomics),
    height(height) {}

void GlPainter::setColor(const QColor &color) {
    painter->setPen(color);
}

void GlPainter::asyncLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2-y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        int lockIdx = y1 * height + x1;
        while(!atomics[lockIdx].testAndSetAcquire(0, 1)) {}
        painter->drawPoint(x1, y1);
        atomics[lockIdx] = 0;
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



