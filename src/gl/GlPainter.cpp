#include "GlPainter.h"

#include <cmath>

#include "Scene.h"
#include "../math/Algorythm.h"
#include "../common/const.h"


GlPainter::GlPainter(QImage* world, QAtomicInt *atomics, volatile int *zBuffer, BuffPoint *tBuffer, int width, int height)
        : world(world), atomics(atomics), zBuffer(zBuffer), tBuffer(tBuffer), width(width), height(height) {
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

    int minX = std::min(std::min(t0.x, t1.x), t2.x);
    int maxX = std::max(std::max(t0.x, t1.x), t2.x);
    int minY = std::min(std::min(t0.y, t1.y), t2.y);
    int maxY = std::max(std::max(t0.y, t1.y), t2.y);

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j <= maxY; ++j) {
            if (i >= width || i < 0 || j > height || j < 0) {
                continue;
            }
            QVector3D bar = toBarycentric2D_int(t0, t1, t2, (float)i, (float)j);
            if (bar.x() < 0 || bar.y() < 0 || bar.z() < 0) {
                continue;
            }
            int lockIdx = j * width + i;

            Vec3i p = (t0 * bar.x() + t1 * bar.y() + t2 * bar.z());
            lock(lockIdx);
            if (zBuffer[lockIdx] > p.z) {
                zBuffer[lockIdx] = p.z;
                tBuffer[lockIdx] = {face, bar};
            }
            unlock(lockIdx);
        }
    }




//    if (t0.y==t1.y && t0.y==t2.y) return;
//    if (t0.y>t1.y) std::swap(t0, t1);
//    if (t0.y>t2.y) std::swap(t0, t2);
//    if (t1.y>t2.y) std::swap(t1, t2);
//    int total_height = t2.y-t0.y;
//    for (int i=0; i<total_height; i++) {
//        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
//        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
//        float alpha = (float)i/total_height;
//        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
//        QVector3D A = t0.toVector3D() + (t2-t0)*alpha;
//        QVector3D B = second_half ? t1.toVector3D() + (t2-t1)*beta : t0.toVector3D() + (t1-t0)*beta;
//        if (A.x()>B.x()) std::swap(A, B);
//        for (int j=A.x(); j<=B.x(); j++) {
//            float phi = B.x()==A.x() ? 1. : (float)(j-A.x())/(float)(B.x()-A.x());
//            QVector3D P = A + (B - A) * phi;
//            Vec3i Pi = P;
//            int lockIdx = Pi.x+Pi.y*width;
//            lock(lockIdx);
//            if (zBuffer[lockIdx] > Pi.z) {
//                zBuffer[lockIdx] = Pi.z;
//                tBuffer[lockIdx] = {face, P.x(), P.y()};
//               // QVector3D bar = toBarycentric2D_int(t0, t1, t2, P.x(), P.y());
////                tBuffer[lockIdx] = {face, (float)Pi.x, (float)Pi.y};
//
//            }
//            unlock(lockIdx);
//        }
//    }
}

void GlPainter::putPoint(int idx) {
    int y = idx / width;
    int x = idx % width;
    QRgb* line = (QRgb*)world->scanLine(y);
    line[x] = QColor::fromRgb(255, 255, 255).rgb();
}

float texelToNormal(QRgb texel) {
    return ((float)texel / 255) * 2 - 1;
}


//Vec3i getPerspectiveDiffuse(const QVector3D& bar, )

void GlPainter::putLightPoint(const Model3D *model, const Face &face, int px, const QVector3D &bar,
                              const QVector3D &inverseLight, const QVector3D &viewFront) {
//    int y = pixel / width;
//    int x = pixel % width;

    Mtl* mtl = face.getMtl();
    QImage& diffuseImage = mtl->diffuseImage;
    QImage& normalImage = mtl->normalImage;
    QImage& specularImage = mtl->specularImage;
    QImage& emissionImage = mtl->emissionImage;


//    if (bc_screen.x() < 0 || bc_screen.y() < 0 || bc_screen.z() < 0 ||
//        bc_screen.x() > 1 || bc_screen.y() > 1 || bc_screen.z() > 1)  {
//        return;
//    }

    const QVector3D& tA = *face[0].texture;
    const QVector3D& tB = *face[1].texture;
    const QVector3D& tC = *face[2].texture;


    QVector3D bc_clip = {bar.x() / face[0].screen.w(),
                         bar.y() / face[1].screen.w(),
                         bar.z() / face[2].screen.w()};
    bc_clip = bc_clip / (bc_clip.x() + bc_clip.y() + bc_clip.z());

//    if (bc_clip.x() < 0 || bc_clip.y() < 0 || bc_clip.z() < 0 ||
//        bc_clip.x() > 1 || bc_clip.y() > 1 || bc_clip.z() > 1) {
//        return;
//        //exit(0);
//    }

    // sdkwa;sTODO: Fix successfull, remove
    if (isnanf(bc_clip.x()) || isnanf(bc_clip.y()) || isnanf(bc_clip.z())) {
        //qDebug() << bc_screen;
        //return;
        return;
    }

    QVector3D t = (tA * bc_clip.x() + tB * bc_clip.y() + tC * bc_clip.z());

//    if (t.x() < 0 || t.y() < 0 || t.z() < 0 || t.x() > 1 || t.y() > 1 || t.z() > 1) {
//        qDebug() << "---";
//        qDebug() << t << bc_clip << tA << tB << tC;
//        return;
//    }

    Vec3i diffuseColor = !diffuseImage.isNull() ? texel(diffuseImage, t) : DIFFUSE_DEF_COLOR;

    Vec3i emissionColor = !emissionImage.isNull() ? texel(emissionImage, t) : Vec3i{0, 0, 0};

    Vec3i lightColor = LIGHT_COLOR;

    Vec3i ambient = diffuseColor * AMBIENT_WEIGHT;

    QVector3D n;
    if (!normalImage.isNull()) {
        Vec3i v = texel(normalImage, t);
        n = QVector3D{texelToNormal(v.x), texelToNormal(v.y), texelToNormal(v.z)}.normalized();
    } else {
        const QVector3D& nA = *face[0].normal;
        const QVector3D& nB = *face[1].normal;
        const QVector3D& nC = *face[2].normal;
        n = (nA * bc_clip.x() + nB * bc_clip.y() + nC * bc_clip.z()).normalized();
    }

    Vec3i diffuse = diffuseColor * DIFFUSE_WEIGHT * std::max(QVector3D::dotProduct(n, inverseLight), 0.f);

    QVector3D lightReflect = reflect(inverseLight, n).normalized();
    float specularWeight = SPECULAR_WEIGHT;
    if (!specularImage.isNull()) {
        specularWeight = (float) texel(specularImage, t).x / 255;
    }

    Vec3i specular = lightColor * specularWeight *
                     powf(std::max(QVector3D::dotProduct(lightReflect, viewFront), 0.f) , SPECULAR_POWER);

    screen[px] = (ambient + diffuse + emissionColor + specular).toRgb();
}



