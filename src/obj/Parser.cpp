#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "Parser.h"
#include "../common/const.h"

QVector3D getPoint(const QStringList& parseLine);
Vec3i getColor(const QStringList& parseLine);
void processFaceIndexes(const QStringList &parseLine, QList<IdxFace> *idxFaces, Mtl *faceMtl);
QString getMtlName(const QStringList& parseLine);
void loadMtlLibrary(const QString &modelDir, const QString &path, MtlContext &mtlContext);

float normalizeTexture(float f) {
    // x.7 -> 0.7
    // -x.7 -> 0.3
    return f >= 0 ? f - floorf(f) : 1 - normalizeTexture(-f);
}

ObjModel *parseObjFile(const QString &modelDir, const QString &path, MtlContext &mtlContext) {
    Mtl* currentMtl = mtlContext.getDefMtl();
    auto *points = new QList<QVector3D>{};
    auto *normals = new QList<QVector3D>{};
    auto *textures = new QList<QVector3D>{};
    auto *idxFaces = new QList<IdxFace>{};
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QStringList list = in.readLine().split(' ');
            if (list[0] == "mtllib") {
                loadMtlLibrary(modelDir, modelDir + "/" + getMtlName(list), mtlContext);
            } else if (list[0] == "usemtl") {
                currentMtl = mtlContext.getMtl(getMtlName(list));
            }
            else if (list[0] == 'v') {
                points->append(getPoint(list));
            } else if (list[0] == "vn") {
                normals->append(getPoint(list).normalized());
            } else if (list[0] == "vt") {
                QVector3D texture = getPoint(list);
                texture.setX(normalizeTexture(texture.x()));
                texture.setY(normalizeTexture(texture.y()));
                texture.setZ(normalizeTexture(texture.z()));
                textures->append(texture);
            }
            else if (list[0] == 'f') {
                processFaceIndexes(list, idxFaces, currentMtl);
            }
        }
        file.close();
    } else {
        qDebug() << "Cannot open model file";
    }
    return new ObjModel(points, normals, textures, idxFaces);
}

struct MtlContent {
    bool Ns = false;
    bool Kd = false;
    bool Ks = false;
    bool textureSearch = false; // Extra texture searching
};

void addMtl(const QString &modelDir, Mtl *mtl, QMap<QString, Mtl *> &mtls, const MtlContent &content) {
    QStringList suffixes = {"", "_baseColor", "_basecolor", "_diffuse"};
    QStringList exts = {".png", ".jpg", ".JPG", ".jpeg"};

    mtl->specularExp = content.Ns ? mtl->specularExp : SPECULAR_POWER;
    mtl->diffuseColor = content.Kd ? mtl->diffuseColor : DIFFUSE_DEF_COLOR;
    mtl->specularColor = content.Ks ? mtl->specularColor : LIGHT_COLOR;
    if (mtl->diffuseImage.isNull() && content.textureSearch) {
        qDebug() << "Bad mtl detected...";
        // Try to search image in textures folder
        QStringList names = {mtl->name, mtl->name.toLower(), mtl->name.toUpper()};
        for (QString& name : names) {
            for (QString& suffix : suffixes) {
                for (QString& ext : exts) {
                    QString path = modelDir + "/textures/" + name + suffix + ext;
                    if (QFile::exists(path)) {
                        mtl->diffuseImage = QImage{path};
                        mtls.insert(mtl->name, mtl);
                        return;
                    }
                }
            }
        }

    }
    mtls.insert(mtl->name, mtl);
}

void loadMtlLibrary(const QString &modelDir, const QString &path, MtlContext &mtlContext) {
    QMap<QString, Mtl*> mtls;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed load mtl library " << path;
        return;
    }
    QTextStream in(&file);

    Mtl* current = nullptr;
    MtlContent content;
    content.textureSearch = mtlContext.isTextureSearch();

    while (!in.atEnd()) {
        QStringList list = in.readLine().split(' ');
        if (list[0] == "newmtl") {
            if (current) {
                addMtl(modelDir, current, mtls, content);
            }
            current = new Mtl{};
            current->name = list[1];
        } else if (list[0] == "map_Kd" && current) {
            current->diffuseImage = QImage{modelDir + "/" + list[1]};
        } else if (list[0] == "map_Ks" && current) {
            current->specularImage = QImage{modelDir + "/" + list[1]};
        } else if (list[0] == "map_Ke" && current) {
            current->emissionImage = QImage{modelDir + "/" + list[1]};
        } else if (list[0] == "Ns" && current) {
            content.Ns = true;
            current->specularExp = list[1].toFloat();
        } else if (list[0] == "Kd" && current) {
            content.Kd = true;
            current->diffuseColor = getColor(list);
        } else if (list[0] == "Ks" && current) {
            content.Ks = true;
            current->specularColor = getColor(list);
        }
    }
    if (current) {
        addMtl(modelDir, current, mtls, content);
    }

    mtlContext.addMtls(mtls);
    qDebug() << "Use mtl library: " << path;
}

QString getMtlName(const QStringList& parseLine) {
    return parseLine.mid(1).join(' ');
}

Vec3i getColor(const QStringList& parseLine) {
    QVector3D v = getPoint(parseLine);
    return {(int)(255 * v.x()), (int)(255 * v.y()), (int)(255 * v.z())};
}

QVector3D getPoint(const QStringList& parseLine) {
    int lastIdx = parseLine.length() - 1;
    float x = parseLine[1].toFloat();
    float y = lastIdx < 2 ? 0 : parseLine[2].toFloat();
    float z = lastIdx < 3 ? 0 : parseLine[3].toFloat();
    return QVector3D{x, y, z};
}

void processFaceIndexes(const QStringList &parseLine, QList<IdxFace> *idxFaces, Mtl *faceMtl) {
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
        IdxFace idxFace{faceMtl};
        idxFace.append({idxVertexes[0], idxVertexes[i], idxVertexes[i + 1]});
        idxFaces->append(idxFace);
    }
}
