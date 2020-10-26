#ifndef GLPAINTER_SCENE_H
#define GLPAINTER_SCENE_H

#include <QtCore/QObject>
#include <QtGui/QPainter>
#include <QtCore/QMutex>
#include "GlPainter.h"
#include "Camera.h"
#include "../obj/Model3D.h"

class Scene : public QObject {
    Q_OBJECT

private:
    const QString modelPath = "Ship/Model.obj";
    QAtomicInt* atomics = nullptr;
    volatile int* zBuffer = nullptr;

    QList<Model3D*> models;

    GlPainter painter;
    int width;
    int height;

    Camera* camera;

    static QVector3D lightFront;

    void paintModel(Model3D *model);

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
