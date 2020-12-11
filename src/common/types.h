#ifndef GLPAINTER_TYPES_H
#define GLPAINTER_TYPES_H

#include <QVector3D>
#include <QtGui/QVector4D>
#include <QtGui/QVector2D>
#include <QtGui/QImage>

struct Mtl {
    QString name;

    QImage diffuseImage;
    QImage normalImage;
    QImage specularImage;
    QImage emissionImage;

    Vec3i ambientColor;
    Vec3i diffuseColor;
};

template<typename T>
class _Face : public QList<T> {
    Mtl* mtl;
public:
    _Face(Mtl* mtl) : mtl(mtl) { };
    Mtl* getMtl() const {return mtl; }
};

struct Vertex {
    const QVector3D* point;
    const QVector3D* normal;
    const QVector3D* texture;
    QVector4D screen;
    Vec3i screen2D;
};

typedef _Face<Vertex> Face;

struct IdxVertex {
    int pointIdx;
    int normalIdx;
    int textureIdx;
};

typedef _Face<IdxVertex> IdxFace;


struct BuffPoint {
    Face* face;
    QVector3D bar;
};


#endif //GLPAINTER_TYPES_H
