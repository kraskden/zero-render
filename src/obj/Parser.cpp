#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>
#include "Parser.h"

QVector3D getPoint(const QStringList& parseLine);
void processFaceIndexes(const QStringList& parseLine, QList<IdxFace>* idxFaces);

ObjModel *parseObjFile(const QString& path) {
    auto *points = new QList<QVector3D>{};
    auto *normals = new QList<QVector3D>{};
    auto *idxFaces = new QList<IdxFace>{};
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QStringList list = in.readLine().split(' ');
            if (list[0] == 'v') {
                points->append(getPoint(list));
            } else if (list[0] == "vn") {
                normals->append(getPoint(list));
            }
            else if (list[0] == 'f') {
                processFaceIndexes(list, idxFaces);
            }
        }
        file.close();
        qDebug() << "Model loaded successfully";
    } else {
        qDebug() << "Cannot open model file";
    }
    return new ObjModel(points, normals, idxFaces);
}

QVector3D getPoint(const QStringList& parseLine) {
    return {parseLine[1].toFloat(), parseLine[2].toFloat(), parseLine[3].toFloat()};
}

void processFaceIndexes(const QStringList& parseLine, QList<IdxFace>* idxFaces) {
    QList<IdxVertex> idxVertexes;
    for (int i = 1; i < parseLine.length(); ++i) {
        QString pointEntry = parseLine[i];
        QStringList idxes = pointEntry.split('/');
        idxVertexes.append({idxes[0].toInt(), idxes[2].toInt()});
    }
    for (int i = 1; i < idxVertexes.length() - 1; ++i) {
        QList<IdxVertex> idxFace = {idxVertexes[0], idxVertexes[i], idxVertexes[i + 1]};
        idxFaces->append(idxFace);
    }
//
//    idxFaces->append(idxVertexes);
}
