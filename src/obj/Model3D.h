#ifndef GLPAINTER_MODEL3D_H
#define GLPAINTER_MODEL3D_H


#include <QtGui/QMatrix4x4>
#include <QtGui/QImage>
#include <QtCore/QSettings>
#include "ObjModel.h"
#include "MtlContext.h"

class Model3D : public MtlContext {
    ObjModel* objModel = nullptr;
    QMatrix4x4 worldMatrix;

    QMap<QString, Mtl*> mtls;
    Mtl* defMtl;

public:
    explicit Model3D(const QString& modelDir);

    Model3D(ObjModel* objModel, QMatrix4x4 worldMatrix) {
        this->objModel = objModel;
        this->worldMatrix = worldMatrix;
    }


    QList<Face>& getFaces() const {return objModel->getFaces(); }
    const QMatrix4x4& getWorldMatrix() const {return worldMatrix;}
    const ObjModel* obj() const {return objModel;}

    Mtl* getDefMtl() override {return defMtl; }
    void addMtls(const QMap<QString, Mtl*>& data) override {mtls.insert(data); }
    Mtl* getMtl(const QString& name) override {return mtls.value(name, defMtl); }

    ~Model3D() {
        delete objModel;
        delete defMtl;
        qDeleteAll(mtls);
    }

private:
    void loadDefMtl(const QString& modelDir, const QSettings &settings);
};


#endif //GLPAINTER_MODEL3D_H
