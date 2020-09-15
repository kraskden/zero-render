#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>
#include "Parser.h"

QVector3D getPoint(const QStringList& parseLine);
void processPolygon(const QStringList& parseLine, QList<QList<int>>* polygons);

ObjModel *parseObjFile(const QString& path) {
    auto *points = new QList<QVector3D>{};
    auto *polygons = new QList<QList<int>>{};
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QStringList list = in.readLine().split(' ');
            if (list[0] == 'v') {
                points->append(getPoint(list));
            } else if (list[0] == 'f') {
                processPolygon(list, polygons);
            }
        }
        file.close();
        qDebug() << "Model loaded successfully";
    } else {
        qDebug() << "Cannot open model file";
    }
    return new ObjModel(points, polygons);
}

QVector3D getPoint(const QStringList& parseLine) {
    return {parseLine[1].toFloat(), parseLine[2].toFloat(), parseLine[3].toFloat()};
}

void processPolygon(const QStringList& parseLine, QList<QList<int>>* polygons) {
    QList<int> pointList;
    for (int i = 1; i < parseLine.length(); ++i) {
        QString pointEntry = parseLine[i];
        int idx = pointEntry.indexOf(QChar('/'));
        if (idx != -1) {
            pointEntry.remove(idx, pointEntry.length() - idx);
        }
        pointList.append(pointEntry.toInt());
    }
    polygons->append(pointList);
}
