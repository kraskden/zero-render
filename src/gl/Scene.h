#ifndef GLPAINTER_SCENE_H
#define GLPAINTER_SCENE_H

#include <QtCore/QObject>
#include <QtGui/QPainter>
#include <QtCore/QMutex>
#include "GlPainter.h"
#include "Camera.h"
#include "../obj/Model3D.h"
#include "LightSource.h"

class Scene : public QObject {
    Q_OBJECT
private:

    QAtomicInt* atomics = nullptr;
    volatile int* zBuffer = nullptr;
    BuffPoint* tBuffer = nullptr;

    Model3D* model = nullptr;

    GlPainter painter;
    int width;
    int height;

    bool isDebug = false;

    Camera* camera;
    LightSource* lightSource;

    static QVector3D lightFront;

    void paintModel(Model3D *model);

public slots:
    void repaint(QPainter *qPainter);

public:
    explicit Scene(int width, int height, QObject *parent);

    void setWidth(int width);

    void setHeight(int height);

    void setCamera(Camera* camera);

    void setLightSource(LightSource *lightSource);

    void setModel(Model3D *model);

    void toggleDebug() {isDebug = !isDebug; }

    ~Scene() override;


private:
    void debugPrint(QPainter *);
    void createBuffers();
};


#endif //GLPAINTER_SCENE_H
