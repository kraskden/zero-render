#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QDebug>
#include "Parser.h"

QVector3D getPoint(const QStringList& parseLine);
void processFaceIndexes(const QStringList& parseLine, QList<IdxFace>* idxFaces);

ObjModel *parseObjFile(const QString& path) {
    auto *points = new QList<QVector3D>{};
    auto *normals = new QList<QVector3D>{};
    auto *textures = new QList<QVector3D>{};
    auto *idxFaces = new QList<IdxFace>{};
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QStringList list = in.readLine().split(' ');
            if (list[0] == 'v') {
                points->append(getPoint(list));
            } else if (list[0] == "vn") {
                normals->append(getPoint(list).normalized());
            } else if (list[0] == "vt") {
                textures->append(getPoint(list));
            }
            else if (list[0] == 'f') {
                processFaceIndexes(list, idxFaces);
            }
        }
        file.close();
    } else {
        qDebug() << "Cannot open model file";
    }
    return new ObjModel(points, normals, textures, idxFaces);
}

QVector3D getPoint(const QStringList& parseLine) {
    int lastIdx = parseLine.length() - 1;
    float x = parseLine[1].toFloat();
    float y = lastIdx < 2 ? 0 : parseLine[2].toFloat();
    float z = lastIdx < 3 ? 0 : parseLine[3].toFloat();
    return QVector3D{x, y, z};
}

void processFaceIndexes(const QStringList& parseLine, QList<IdxFace>* idxFaces) {
    QList<IdxVertex> idxVertexes;
    for (int i = 1; i < parseLine.length(); ++i) {
        QString pointEntry = parseLine[i];
        QStringList idxes = pointEntry.split('/');
        if (idxes.length() != 3) {
            continue;
        }
        idxVertexes.append({idxes[0].toInt(), idxes[2].toInt(), idxes[1].toInt()});
    }
    for (int i = 1; i < idxVertexes.length() - 1; ++i) {
        QList<IdxVertex> idxFace = {idxVertexes[0], idxVertexes[i], idxVertexes[i + 1]};
        idxFaces->append(idxFace);
    }
}
