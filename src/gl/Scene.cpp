#include <QDebug>
#include <QVector2D>

#include "Scene.h"
#include <cmath>
#include <QtCore/QMutex>
#include "../math/Matrix4D.h"
#include "../obj/Parser.h"
#include "../common/const.h"
#include "../common/tbuffer.h"
#include "../math/Algorythm.h"

#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrent>

QVector3D Scene::lightFront = QVector3D{0, 0, -1};

Scene::Scene(int width, int height, QObject *parent) :
        QObject(parent), painter(nullptr, nullptr, nullptr, nullptr, 0), width(width), height(height) {

    models.append(new Model3D(parseObjFile(modelPath), matrix::identify()));
    // TODO: Light source movement
//    models.append(new Model3D(parseObjFile("Cube/Model.obj"),
//                          matrix::scale_move({0.2, 0.2, 0.2}, {0, 0, 10}),
//                              ModelType::ENTITY));
    createBuffers();
}

Scene::~Scene() {
    qDeleteAll(models.begin(), models.end());
}

void Scene::repaint(QPainter *qPainter) {
    std::fill(zBuffer, zBuffer + width * height, std::numeric_limits<int>::max());
    std::fill(tBuffer, tBuffer + width * height, nullptr);
    QImage world = QImage(width, height, QImage::Format_RGB32);
    world.fill(QColorConstants::Cyan);

    this->painter = GlPainter(&world, atomics, zBuffer, tBuffer, width);

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

void Scene::paintModel(Model3D *model) {
    QMatrix4x4 viewport = matrix::viewport(width, height);
    QMatrix4x4 view = camera->getViewMatrix();
    QMatrix4x4 projection = matrix::projection_rel(width * 1.0 / height, FOV, Z_NEAR, Z_FAR);
    QMatrix4x4 projectionView = projection  * view * model->getWorldMatrix();

    QVector3D inverseLight = lightSource->getFront();
//    QVector3D inverseLight = lightFront.normalized();
    inverseLight *= -1.f;
    QVector3D viewFront = camera->getFront().normalized();

    QList<Face>& faces = model->getFaces();
    QFuture<void> computeLoop = QtConcurrent::map(faces.begin(), faces.end(), [&](Face& face) -> void {
        for (int i = 0 ; i < 3; ++i) {
            QVector4D proj = projectionView * QVector4D(*face[i].point, 1);
            if (is_vec_drop(proj)) return;
            face[i].screen = viewport * proj;
            norm_vec(face[i].screen);
        }
        QVector3D a = (face[2].screen - face[0].screen).toVector3D();
        QVector3D b = (face[1].screen - face[0].screen).toVector3D();
        float visibility = a.x() * b.y() - a.y() * b.x();
        if (visibility > 0 ) {
            this->painter.fillTBuffer(&face, face[0].screen.toVector3D(), face[1].screen.toVector3D(),
                                      face[2].screen.toVector3D());
        }
    });
    computeLoop.waitForFinished();
    Face** start = tBuffer;
    Face** end = tBuffer + width * height;
    QFuture<void> paintLoop = QtConcurrent::map(start, end, [&](Face*& face) -> void {
        if (!face) {return;}
        int idx = &face - start;
        painter.putLightPoint(*face, idx, inverseLight, viewFront);
    });
    paintLoop.waitForFinished();
}

void Scene::setCamera(Camera *camera) {
    this->camera = camera;
}

QString float2str(float f, int prec = 2) {
    return QString::number(f, 'f', prec);
}

float rad2deg(float angle) {
    return (float)(angle * 180 / M_PI);
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

    QString text = QString("Eye: %1\nFront: %2\nPitch: %3\nYaw: %4\nMove speed:  %5\nRotate speed: %6\n"
                           "Light Front: %7\nAzimuth: %8\nAltitude: %9")
            .arg(vec2str(eye))
            .arg(vec2str(camera->getFront()))
            .arg(deg2str(rad2deg(camera->getPitch())))
            .arg(deg2str(rad2deg(camera->getYaw())))
            .arg(float2str(camera->getMovementSpeed()))
            .arg( float2str(rad2deg(camera->getRotateSpeed()), 1))
            .arg(vec2str(lightSource->getFront()))
            .arg(deg2str(rad2deg(lightSource->getAzimuth())))
            .arg(deg2str(rad2deg(lightSource->getAltitude())));

    qPainter->drawText(QRect(0, 0, 1000, 1000), text);

    qPainter->setPen(prevColor);
}

void Scene::createBuffers() {
    delete [] zBuffer;
    delete [] atomics;
    delete [] tBuffer;
    atomics = new QAtomicInt[width * height];
    zBuffer = new volatile int[width * height];
    tBuffer = new Face*[width * height];
}

void Scene::setLightSource(LightSource *lightSource) {
    Scene::lightSource = lightSource;
}
