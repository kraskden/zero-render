#ifndef GLPAINTER_SCENE_H
#define GLPAINTER_SCENE_H

#include <QtCore/QObject>
#include <QtGui/QPainter>
#include <QtCore/QMutex>
#include "../obj/ObjModel.h"
#include "GlPainter.h"
#include "Camera.h"

class Scene : public QObject {
    Q_OBJECT

private:
    const QString modelPath = "Ship/Model.obj";
    QAtomicInt* atomics = nullptr;
    volatile int* zBuffer = nullptr;

    QList<ObjModel*> models;

    GlPainter painter;
    int width;
    int height;

    Camera* camera;

    static QVector3D lightFront;

    void paintModel(ObjModel* model);

public slots:
    void repaint(QPainter *qPainter);

public:
    explicit Scene(int width, int height, QObject *parent);

    void setWidth(int width);

    void setHeight(int height);

    void setCamera(Camera* camera);

    ~Scene() override;


private:
    void debugPrint(QPainter *);
    void createBuffers();
};


#endif //GLPAINTER_SCENE_H
