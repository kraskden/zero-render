#ifndef GLPAINTER_OBJMODEL_H
#define GLPAINTER_OBJMODEL_H

#include <QList>
#include <QVector3D>
#include <QtGui/QVector4D>
#include <QtGui/QVector2D>
#include "../math/Vec3i.h"

#include "../common/types.h"

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
