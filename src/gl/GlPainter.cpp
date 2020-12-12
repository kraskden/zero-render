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

void GlPainter::fillTBuffer(Face *face, const Vec3i &t0, const Vec3i &t1, const Vec3i &t2) {

    BuffPoint bp;
    bp.face = face;

    int minX = std::min(std::min(t0.x, t1.x), t2.x);
    int maxX = std::max(std::max(t0.x, t1.x), t2.x);
    int minY = std::min(std::min(t0.y, t1.y), t2.y);
    int maxY = std::max(std::max(t0.y, t1.y), t2.y);

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j <= maxY; ++j) {
            if (i >= width || i < 0 || j > height || j < 0) {
                continue;
            }
            bp.bar = toBarycentric2D(*face, i, j);
            const QVector3D& bar = bp.bar;
            if (bar.x() < 0 || bar.y() < 0 || bar.z() < 0) {
                continue;
            }
            int lockIdx = j * width + i;

            Vec3i p = (t0 * bar.x() + t1 * bar.y() + t2 * bar.z());
            lock(lockIdx);
            if (zBuffer[lockIdx] > p.z) {
                zBuffer[lockIdx] = p.z;
                tBuffer[lockIdx] = bp;
            }
            unlock(lockIdx);
        }
    }
}

float texelToNormal(QRgb texel) {
    return ((float)texel / 255) * 2 - 1;
}



void GlPainter::putLightPoint(const Model3D *model, const Face &face, int px, const QVector3D &bar,
                              const QVector3D &inverseLight, const QVector3D &viewFront) {

    Mtl* mtl = face.getMtl();
    QImage& diffuseImage = mtl->diffuseImage;
    QImage& normalImage = mtl->normalImage;
    QImage& specularImage = mtl->specularImage;
    QImage& emissionImage = mtl->emissionImage;

    const QVector3D& tA = *face[0].texture;
    const QVector3D& tB = *face[1].texture;
    const QVector3D& tC = *face[2].texture;

    QVector3D bc_clip = {bar.x() / face[0].screen.w(),
                         bar.y() / face[1].screen.w(),
                         bar.z() / face[2].screen.w()};
    bc_clip = bc_clip / (bc_clip.x() + bc_clip.y() + bc_clip.z());

    QVector3D t = (tA * bc_clip.x() + tB * bc_clip.y() + tC * bc_clip.z());

    Vec3i diffuseColor = !diffuseImage.isNull() ? texel(diffuseImage, t) : mtl->diffuseColor;

    Vec3i emissionColor = !emissionImage.isNull() ? texel(emissionImage, t) : Vec3i{0, 0, 0};

    Vec3i lightColor = mtl->specularColor;

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
                     powf(std::max(QVector3D::dotProduct(lightReflect, viewFront), 0.f) , mtl->specularExp);

    screen[px] = (ambient + diffuse + emissionColor + specular).toRgb();
}



