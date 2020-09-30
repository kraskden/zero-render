#include <QDebug>
#include <QVector2D>

#include "Scene.h"
#include <cmath>
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

void norm_vec(QVector4D& vec) {
    vec.setX(vec.x() / vec.w());
    vec.setY(vec.y() / vec.w());
    vec.setZ(vec.z() / vec.w());
    vec.setW(1.0);
}


void Scene::paintModel(ObjModel *model) {
    debugPrint();
    QVector3D eye = camera->getEye();
    QVector3D center = camera->getCenter();
    QVector3D up{0, 1, 0};

    QMatrix4x4 mod = matrix::scale(QVector3D{1,1,1});
    QMatrix4x4 viewport = matrix::viewport(width, height);
    QMatrix4x4 view = matrix::view(eye, center, up);
    QMatrix4x4 projection = matrix::projection_my((eye - center).normalized().length());

    QMatrix4x4 final = viewport * projection  * view * mod;

    for (Face face : model->getFaces()) {
        for (int i = 0; i < 3; ++i) {
            QVector4D fst = final * QVector4D(*face[i], 1);
            QVector4D snd = final *  QVector4D(*face[(i + 1) % 3], 1);

            norm_vec(fst); norm_vec(snd);

            painter.line(fst.x(), fst.y(), snd.x(), snd.y());
        }
    }
}

void Scene::setCamera(Camera *camera) {
    this->camera = camera;
}

QString vec2str(const QVector3D& vec) {
    return QString("[ %1, %2, %3]")
        .arg(QString::number(vec.x(), 'f', 2))
        .arg(QString::number(vec.y(), 'f', 2))
        .arg(QString::number(vec.z(), 'f', 2));
}

void Scene::debugPrint() {
    QPainter* qPainter = painter.qPainter();
    QColor prevColor = qPainter->pen().color();
    qPainter->setPen(QColorConstants::White);

    const QVector3D& eye = camera->getEye();
    const QVector3D& center = camera->getCenter();
    const float radius = camera->getRadius();
    const float angle = (int)(camera->getAngle() * 180 / M_PI) % 360;


    QString text = QString("Radius: %1\nAngle: %2\nEye: %3\nCenter: %4").arg(radius).arg(angle)
            .arg(vec2str(eye))
            .arg(vec2str(center));
    qPainter->drawText(QRect(0, 0, 1000, 100), text);

    qPainter->setPen(prevColor);
}

