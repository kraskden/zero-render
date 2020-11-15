#ifndef GLPAINTER_OBJMODEL_H
#define GLPAINTER_OBJMODEL_H

#include <QList>
#include <QVector3D>
#include <QtGui/QVector4D>
#include "../math/Vec3i.h"

struct Vertex {
    const QVector3D* point;
    const QVector3D* normal;
    const QVector3D* texture;
    QVector4D screen;
};
typedef QList<Vertex> Face;

struct IdxVertex {
    int pointIdx;
    int normalIdx;
    int textureIdx;
};

typedef QList<IdxVertex> IdxFace;

class ObjModel {

    QList<QVector3D>* points;
    QList<QVector3D>* normals;
    QList<QVector3D>* textures;
    QList<Face>* faces;

public:
    ObjModel(QList<QVector3D> *points, QList<QVector3D> *normals, QList<QVector3D> *textures, QList<IdxFace> *idxFaces);

    QList<Face>& getFaces() const;

    virtual ~ObjModel();
};


#endif //GLPAINTER_OBJMODEL_H
