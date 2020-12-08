#include "ObjModel.h"

#include <QDebug>

ObjModel::ObjModel(QList<QVector3D> *points, QList<QVector3D> *normals, QList<QVector3D> *textures,
                   QList<IdxFace> *idxFaces) : points(points), normals(normals), textures(textures) {
    faces = new QList<Face>{};
    int pointsLen = points->size();
    int normalsLen = normals->size();
    int texturesLen = textures->size();
    for (IdxFace& idxFace : *idxFaces) {
        QList<Vertex> shape;
        for (IdxVertex& idxVertex : idxFace) {
            idxVertex.pointIdx < 0 ? idxVertex.pointIdx += pointsLen : idxVertex.pointIdx -= 1;
            idxVertex.normalIdx < 0 ? idxVertex.normalIdx += normalsLen : idxVertex.normalIdx -= 1;
            idxVertex.textureIdx < 0 ? idxVertex.textureIdx += texturesLen : idxVertex.textureIdx -= 1;
            Vertex vtx = {&points->at(idxVertex.pointIdx), &normals->at(idxVertex.normalIdx), &textures->at(idxVertex.textureIdx)};
            shape.append(vtx);
        }
        faces->append(shape);
    }
    delete idxFaces;
}


ObjModel::~ObjModel() {
    delete points;
    delete normals;
    delete faces;
    delete textures;
}

QList<Face>& ObjModel::getFaces() const {
    return *faces;
}