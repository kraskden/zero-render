#ifndef GLPAINTER_OBJMODEL_H
#define GLPAINTER_OBJMODEL_H

#include <QList>
#include <QVector3D>

typedef const QVector3D* Point;
typedef QList<Point> Face;

class ObjModel {

    QList<QVector3D>* points;
    QList<QList<int>>* polygons;
    QList<Face>* faces;

public:
    ObjModel(QList<QVector3D> *points, QList<QList<int>> *polygons);

    QList<Face>& getFaces() const;

    QList<QVector3D>& getPoints() const;

    QList<QList<int>>& getPolygons() const;

    virtual ~ObjModel();
};


#endif //GLPAINTER_OBJMODEL_H
