#include <QDebug>
#include <QVector2D>

#include "Scene.h"
#include "../math/Matrix4D.h"
#include "../obj/Parser.h"

Scene::Scene(int width, int height, QObject *parent) :
    QObject(parent), painter(nullptr), width(width), height(height) {

    models.append(parseObjFile(modelPath));
}

Scene::~Scene() {
    qDeleteAll(models.begin(), models.end());
}

void Scene::repaint() {
    painter.setColor(QColorConstants::Red);
    painter.clean(width, height);
    for (auto model : models) {
        paintModel(model);
    }
}

void Scene::setPainter(QPainter *qPainter) {
    this->painter = GlPainter(qPainter);
}

void Scene::setWidth(int width) {
    Scene::width = width;
}

void Scene::setHeight(int height) {
    Scene::height = height;
}

void Scene::paintModel(ObjModel *model) {

   QMatrix4x4 viewport = matrix::scale_move(
           QVector2D{(float)(width / 2.0), (float)(height / - 2.0)},
           QVector2D{(float)(width / 2.0), (float)(height / 2.0)}
           );

    for (Face face : model->getFaces()) {
        for (int i = 0; i < 3; ++i) {
            const QVector3D& fst = viewport * *face[i];
            const QVector3D& snd = viewport * *face[(i + 1) % 3];

            painter.line(fst.x(), fst.y(), snd.x(), snd.y());
        }
    }
}
