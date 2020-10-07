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
        lock(lockIdx);
        painter->drawPoint(x1, y1);
        unlock(lockIdx);
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

void GlPainter::asyncTriangle(QPoint p1, QPoint p2, QPoint p3) {
    if (p1.y() > p2.y()) std::swap(p1, p2);
    if (p1.y() > p3.y()) std::swap(p1, p3);
    if (p2.y() > p3.y()) std::swap(p2, p3);

    float k_start = (float)(p1.x() - p3.x()) / (float)(p1.y() - p3.y());
    float k_end_fst = (float)(p1.x() - p2.x()) / (float)(p1.y() - p2.y());
    float k_end_snd = (float)(p2.x() - p3.x()) / (float)(p2.y() - p3.y());
    for (int yi = p1.y(); yi < p3.y(); ++yi) {
        int x_start = p1.x() + k_start * (yi - p1.y());
        int x_end = yi < p2.y() ? p1.x() + k_end_fst * (yi - p1.y()) : p2.x() + k_end_snd * (yi - p2.y());

        if (x_start > x_end) std::swap(x_start, x_end);
        for (int xi = x_start; xi <= x_end; ++xi) {
           int lockIdx = yi * height + xi;
           lock(lockIdx);
           painter->drawPoint(xi, yi);
           unlock(lockIdx);
        }
    }
}



