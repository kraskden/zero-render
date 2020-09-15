#include "ObjModel.h"

ObjModel::ObjModel(QList<QVector3D> *points, QList<QList<int>> *polygons) : points(points), polygons(polygons) {
    faces = new QList<Face>{};
    int pointsLen = points->size();
    for (const QList<int>& polygon : *polygons) {
        QList<Point> shape;
        for (int pointIdx : polygon) {
            pointIdx < 0 ? pointIdx += pointsLen : pointIdx -= 1;
            shape.append(&points->at(pointIdx));
        }
        faces->append(shape);
    }
}

QList<QVector3D>& ObjModel::getPoints() const {
    return *points;
}

QList<QList<int>>& ObjModel::getPolygons() const {
    return *polygons;
}

ObjModel::~ObjModel() {
    delete points;
    delete polygons;
    delete faces;
}

QList<Face>& ObjModel::getFaces() const {
    return *faces;
}
