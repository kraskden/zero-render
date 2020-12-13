#include "Model3D.h"

#include <QSettings>
#include <QFile>

#include "../common/const.h"
#include "../math/Matrix4D.h"
#include "../obj/Parser.h"

QString fullPath(const QString& dir, const QString& fileName) {
    return dir + "/" + fileName;
}

Model3D::Model3D(const QString& modelDir) {
    settings = new QSettings{modelDir + "/" + MODEL_SETTING_FILE, QSettings::Format::IniFormat};

    QString modelPath = fullPath(modelDir, settings->value("model", "model.obj").toString());
    float scale = settings->value("scale", 1.0).toFloat();
    this->badMtl = settings->value("badMtl", false).toBool();

    if (!QFile::exists(modelPath)) {
        qDebug() << "Model file " + modelPath + " not exists!";
        return;
    }

    loadDefMtl(modelDir, *settings);
    this->worldMatrix = matrix::scale({scale, scale, scale});
    this->objModel = parseObjFile(modelDir, modelPath, *this);
}

void Model3D::loadDefMtl(const QString& modelDir, const QSettings &settings) {
    QString diffusePath = fullPath(modelDir, settings.value("diffuse", "diffuse.png").toString());
    QString normalPath = fullPath(modelDir, settings.value("normal", "normal.png").toString());
    QString specularPath = fullPath(modelDir,settings.value("specular", "specular.png").toString());
    QString emissionPath = fullPath(modelDir, settings.value("emission", "emission.png").toString());

    defMtl = new Mtl();
    defMtl->diffuseImage = QImage{diffusePath};
    defMtl->specularImage = QImage{specularPath};
    defMtl->normalImage = QImage{normalPath};
    defMtl->emissionImage = QImage{emissionPath};

    defMtl->diffuseColor = DIFFUSE_DEF_COLOR;
    defMtl->specularColor = LIGHT_COLOR;
    defMtl->specularExp = SPECULAR_POWER;
}

bool Model3D::isTextureSearch() {
    return this->badMtl;
}
