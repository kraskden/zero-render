#ifndef GLPAINTER_GLPAINTER_H
#define GLPAINTER_GLPAINTER_H


#include <QtGui/QPainter>

class Scene;

class GlPainter {
    QPainter* painter;

public:
    explicit GlPainter(QPainter *painter);

    void clean(int width, int height);
    void setColor(const QColor &color);
    void line(int x1, int y1, int x2, int y2);
    void line(const QPoint& start, const QPoint& end);

    void asyncLine(int x1, int y1, int x2, int y2, Scene* scene);

    QPainter* qPainter() {return painter;}
};


#endif //GLPAINTER_GLPAINTER_H
