#include "Model3D.h"

#include <QSettings>
#include <QFile>

#include "../common/const.h"
#include "../math/Matrix4D.h"
#include "../obj/Parser.h"

QString fullPath(const QString& dir, const QString& fileName) {
    return dir + "/" + fileName;
}

Model3D::Model3D(QString modelDir) {
    QSettings settings{modelDir + "/" + MODEL_SETTING_FILE, QSettings::Format::IniFormat};

    QString modelPath = fullPath(modelDir, settings.value("model", "model.obj").toString());
    QString diffusePath = fullPath(modelDir, settings.value("diffuse", "diffuse.png").toString());
    QString normalPath = fullPath(modelDir, settings.value("normal", "normal.png").toString());
    QString mirroredPath = fullPath(modelDir,settings.value("mirrored", "mirrored.png").toString());
    float scale = settings.value("scale", 1.0).toFloat();

    if (!QFile::exists(modelPath)) {
        qDebug() << "Model file " + modelPath + " not exists!";
        return;
    }

    this->diffuse = new QImage(diffusePath);
    this->normal = new QImage(normalPath);
    this->mirrored = new QImage(mirroredPath);

    this->worldMatrix = matrix::scale({scale, scale, scale});
    this->objModel = parseObjFile(modelPath);
}
