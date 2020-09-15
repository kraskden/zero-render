#ifndef GLPAINTER_SCENE_H
#define GLPAINTER_SCENE_H

#include <QtCore/QObject>
#include <QtGui/QPainter>
#include "../obj/ObjModel.h"
#include "GlPainter.h"


class Scene : public QObject {
    Q_OBJECT

private:
    const QString modelPath = "Head/Model.obj";

    QList<ObjModel*> models;
    GlPainter painter;
    int width;
    int height;

    void paintModel(ObjModel* model);

public:
    explicit Scene(int width, int height, QObject *parent);
    void repaint();

    void setPainter(QPainter* qPainter);

    void setWidth(int width);

    void setHeight(int height);

    ~Scene() override;
};


#endif //GLPAINTER_SCENE_H
