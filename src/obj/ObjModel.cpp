#include "ObjModel.h"

ObjModel::ObjModel(QList<QVector3D> *points, QList<QVector3D> *normals, QList<IdxFace>* idxFaces) : points(points), normals(normals) {
    faces = new QList<Face>{};
    int pointsLen = points->size();
    int normalsLen = normals->size();
    for (IdxFace& idxFace : *idxFaces) {
        QList<Vertex> shape;
        for (IdxVertex& idxVertex : idxFace) {
            idxVertex.pointIdx < 0 ? idxVertex.pointIdx += pointsLen : idxVertex.pointIdx -= 1;
            idxVertex.normalIdx < 0 ? idxVertex.normalIdx += normalsLen : idxVertex.normalIdx -= 1;
            Vertex vtx = {&points->at(idxVertex.pointIdx), &normals->at(idxVertex.normalIdx)};
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
}

QList<Face>& ObjModel::getFaces() const {
    return *faces;
}