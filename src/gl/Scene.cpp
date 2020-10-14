#include <QDebug>
#include <QVector2D>

#include "Scene.h"
#include <cmath>
#include <QtCore/QMutex>
#include "../math/Matrix4D.h"
#include "../obj/Parser.h"
#include "../common/const.h"

#include <QtConcurrent/QtConcurrentMap>

QVector3D Scene::lightFront = QVector3D{0, 0, -1};

Scene::Scene(int width, int height, QObject *parent) :
        QObject(parent), painter(nullptr, nullptr, nullptr, 0), width(width), height(height) {

    models.append(parseObjFile(modelPath));
    createBuffers();
}

Scene::~Scene() {
    qDeleteAll(models.begin(), models.end());
}

void Scene::repaint(QPainter *qPainter) {
    std::fill(zBuffer, zBuffer + width * height, std::numeric_limits<int>::max());
    QImage world = QImage(width, height, QImage::Format_RGB32);
    world.fill(QColorConstants::Black);

    this->painter = GlPainter(&world, atomics, zBuffer, width);

    for (auto model : models) {
        paintModel(model);
    }

    qPainter->drawImage(0, 0, world);
    debugPrint(qPainter);
}

void Scene::setWidth(int width) {
    Scene::width = width;
    createBuffers();
}

void Scene::setHeight(int height) {
    Scene::height = height;
    createBuffers();
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
    QMatrix4x4 mod = matrix::scale(QVector3D{1,1,1});
    QMatrix4x4 viewport = matrix::viewport(width, height);
    QMatrix4x4 view = camera->getViewMatrix();
    QMatrix4x4 projection = matrix::projection_rel(width * 1.0 / height, FOV, Z_NEAR, Z_FAR);

    QMatrix4x4 projectionView = projection  * view * mod;

    QList<QList<Point>>& faces = model->getFaces();
    QFuture<void> paintLoop = QtConcurrent::map(faces.begin(), faces.end(), [&](Face& face) -> void {

        QVector4D projs[3];
        QVector4D screens[3];

        for (int i = 0 ; i < 3; ++i) {
            projs[i] = projectionView * QVector4D(*face[i], 1);
            if (is_vec_drop(projs[i])) return;
            screens[i] = viewport * projs[i];
            norm_vec(screens[i]);
        }

        QVector3D n = QVector3D::crossProduct((*face[2] - *face[0]), (*face[1] - *face[0])).normalized();
        float intensity = QVector3D::dotProduct(n, lightFront);
        float visibility = QVector3D::dotProduct(n, camera->getFront().normalized());
        if (intensity < 0) intensity = 0;

        if (visibility > 0) {
            this->painter.asyncTriangle(screens[0].toVector3D(), screens[1].toVector3D(), screens[2].toVector3D(),
                                        intensity);
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

void Scene::debugPrint(QPainter *qPainter) {
    QColor prevColor = qPainter->pen().color();
    qPainter->setPen(QColorConstants::Red);

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

void Scene::createBuffers() {
    delete [] zBuffer;
    delete [] atomics;
    atomics = new QAtomicInt[width * height];
    zBuffer = new volatile int[width * height];
}