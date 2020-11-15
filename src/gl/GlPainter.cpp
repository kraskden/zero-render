#include "GlPainter.h"

#include <cmath>

#include "Scene.h"
#include "../math/Algorythm.h"
#include "../common/const.h"


GlPainter::GlPainter(QImage* world, QAtomicInt *atomics, volatile int *zBuffer, Face** tBuffer, int width)
        : world(world), atomics(atomics), zBuffer(zBuffer), tBuffer(tBuffer), width(width) {
    if (world) {
        screen = (QRgb*)world->bits();
    }
}

void GlPainter::asyncLine(int x1, int y1, int x2, int y2, QRgb color) {
    int dx = abs(x2 - x1);
    int sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2-y1);
    int sy = y1 < y2 ? 1 : -1;
    int err = dx + dy;
    for (;;) {
        int lockIdx = y1 * width + x1;
        lock(lockIdx);
        world->setPixel(x1, y1, color);
        unlock(lockIdx);
        if (x1 == x2 && y1 == y2) {
            return;
        }
        int err2 = err * 2;
        if (err2 > dy) {
            err += dy;
            x1 += sx;
        }
        if (err2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }

}

void GlPainter::asyncTriangle(Vec3i t0, Vec3i t1, Vec3i t2, float intensity) {
    if (t0.y==t1.y && t0.y==t2.y) return;
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        QVector3D A = t0.toVector3D() + (t2-t0)*alpha;
        QVector3D B = second_half ? t1.toVector3D() + (t2-t1)*beta : t0.toVector3D() + (t1-t0)*beta;
        if (A.x()>B.x()) std::swap(A, B);
        for (int j=A.x(); j<=B.x(); j++) {
            float phi = B.x()==A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
            Vec3i P = (A) + (B-A)*phi;
            int lockIdx = P.x+P.y*width;
            lock(lockIdx);
            if (zBuffer[lockIdx] > P.z) {
                zBuffer[lockIdx] = P.z;
                QRgb* line = (QRgb*)world->scanLine(P.y);
                line[P.x] = QColor::fromRgb(intensity * 255, intensity * 255, intensity * 255).rgb();
            }
            unlock(lockIdx);
        }
    }
}

void GlPainter::fillTBuffer(Face *face, Vec3i t0, Vec3i t1, Vec3i t2) {
    if (t0.y==t1.y && t0.y==t2.y) return;
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        QVector3D A = t0.toVector3D() + (t2-t0)*alpha;
        QVector3D B = second_half ? t1.toVector3D() + (t2-t1)*beta : t0.toVector3D() + (t1-t0)*beta;
        if (A.x()>B.x()) std::swap(A, B);
        for (int j=A.x(); j<=B.x(); j++) {
            float phi = B.x()==A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
            Vec3i P = (A) + (B-A)*phi;
            int lockIdx = P.x+P.y*width;
            lock(lockIdx);
            if (zBuffer[lockIdx] > P.z) {
                zBuffer[lockIdx] = P.z;
                tBuffer[lockIdx] = face;
            }
            unlock(lockIdx);
        }
    }
}

void GlPainter::putPoint(int idx) {
    int y = idx / width;
    int x = idx % width;
    QRgb* line = (QRgb*)world->scanLine(y);
    line[x] = QColor::fromRgb(255, 255, 255).rgb();
}

void GlPainter::putLightPoint(const Model3D *model, const Face &face, int pixel, const QVector3D &inverseLight,
                              const QVector3D &viewFront) {
    int y = pixel / width;
    int x = pixel % width;
    QImage* diffuseImage = model->getDiffuse();

    QVector3D barycentric = toBarycentric(face, (float)x, (float)y);
    const QVector3D& tA = *face[0].texture;
    const QVector3D& tB = *face[1].texture;
    const QVector3D& tC = *face[2].texture;
    QVector3D t = (tA * barycentric.x() + tB * barycentric.y() + tC * barycentric.z()).normalized();
//    printf("%f %f %f %d\n", t.x(), t.y(), t.z(), isnanf(t.x()));

    Vec3i diffuseColor = texel(diffuseImage, t, DIFFUSE_DEF_COLOR);
//    Vec3i diffuseColor = DIFFUSE_DEF_COLOR;

    Vec3i lightColor = LIGHT_COLOR;

    Vec3i ambient = diffuseColor * AMBIENT_WEIGHT;

    const QVector3D& nA = *face[0].normal;
    const QVector3D& nB = *face[1].normal;
    const QVector3D& nC = *face[2].normal;
    QVector3D n = (nA * barycentric.x() + nB * barycentric.y() + nC * barycentric.z()).normalized();
    Vec3i diffuse = diffuseColor * DIFFUSE_WEIGHT * std::max(QVector3D::dotProduct(n, inverseLight), 0.f);

    QVector3D lightReflect = reflect(inverseLight, n).normalized();
    Vec3i specular = lightColor * SPECULAR_WEIGHT *
                     powf(std::max(QVector3D::dotProduct(lightReflect, viewFront), 0.f) , SPECULAR_POWER);

    screen[pixel] = (ambient + diffuse + specular).toRgb();
}



