#include <QDebug>
#include <QVector2D>

#include "Scene.h"
#include <cmath>
#include <QtCore/QMutex>
#include "../math/Matrix4D.h"
#include "../obj/Parser.h"
#include "../common/const.h"

#include <QtConcurrent/QtConcurrentMap>

Scene::Scene(int width, int height, QObject *parent) :
    QObject(parent), painter(nullptr, nullptr, 0), width(width), height(height) {

    models.append(parseObjFile(modelPath));
    createAtomics();
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
    this->painter = GlPainter(qPainter, atomics, height);
}

void Scene::setWidth(int width) {
    Scene::width = width;
    createAtomics();
}

void Scene::setHeight(int height) {
    Scene::height = height;
    createAtomics();
}

void norm_vec(QVector4D& vec) {
    vec.setX(vec.x() / vec.w());
    vec.setY(vec.y() / vec.w());
    vec.setZ(vec.z() / vec.w());
    vec.setW(1.0);
}

bool is_vec_drop(const QVector4D& vec) {
    float w = abs(vec.w());
    return (abs(vec.x()) > w) || (abs(vec.y()) > w) || (abs(vec.z()) > w);
}


void Scene::paintModel(ObjModel *model) {
    debugPrint();
    QMatrix4x4 mod = matrix::scale(QVector3D{1,1,1});
    QMatrix4x4 viewport = matrix::viewport(width, height);
    QMatrix4x4 view = camera->getViewMatrix();
    QMatrix4x4 projection = matrix::projection_rel(width * 1.0 / height, FOV, Z_NEAR, Z_FAR);

    QMatrix4x4 projectionView = projection  * view * mod;

    QList<QList<Point>> faces = model->getFaces();
    QFuture<void> paintLoop = QtConcurrent::map(faces.begin(), faces.end(), [&](Face& face) -> void {
        for (int i = 0; i < 3; ++i) {
            QVector4D fst =  projectionView * QVector4D(*face[i], 1);
            QVector4D snd =  projectionView *  QVector4D(*face[(i + 1) % 3], 1);

            if (is_vec_drop(fst) || is_vec_drop(snd)) {
                continue;
            }

            fst = viewport * fst; snd = viewport * snd;

            norm_vec(fst); norm_vec(snd);
            painter.asyncLine(fst.x(), fst.y(), snd.x(), snd.y());
        }
    });

    paintLoop.waitForFinished();
}

void Scene::setCamera(Camera *camera) {
    this->camera = camera;
}

QString float2str(float f) {
    return QString::number(f, 'f', 2);
}

int rad2deg(float angle) {
    return (int)(angle * 180 / M_PI);
}

QString deg2str(int deg) {
    int real = deg % 360;
    return QString("%1 [%2]").arg(real).arg(deg);
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

    QString text = QString("Eye: %1\nFront: %2\nPitch: %3\nYaw: %4\nMove speed:  %5\nRotate speed: %6")
            .arg(vec2str(eye))
            .arg(vec2str(camera->getFront()))
            .arg(deg2str(rad2deg(camera->getPitch())))
            .arg(deg2str(rad2deg(camera->getYaw())))
            .arg(float2str(camera->getMovementSpeed()))
            .arg( rad2deg(camera->getRotateSpeed()));

    qPainter->drawText(QRect(0, 0, 1000, 1000), text);

    qPainter->setPen(prevColor);
}

void Scene::createAtomics() {
    delete [] atomics;
    atomics = new QAtomicInt[width * height];
}

