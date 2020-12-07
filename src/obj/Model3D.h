#ifndef GLPAINTER_MODEL3D_H
#define GLPAINTER_MODEL3D_H


#include <QtGui/QMatrix4x4>
#include <QtGui/QImage>
#include "ObjModel.h"

class Model3D {
    ObjModel* objModel = nullptr;
    QMatrix4x4 worldMatrix;

    QImage* diffuse = nullptr;
    QImage* normal = nullptr;
    QImage* specular = nullptr;
    QImage* emission = nullptr;

public:
    explicit Model3D(QString modelDir);

    Model3D(ObjModel* objModel, QMatrix4x4 worldMatrix) {
        this->objModel = objModel;
        this->worldMatrix = worldMatrix;
    }

    QImage *getDiffuse() const {
        return diffuse;
    }

    QImage *getNormal() const {
        return normal;
    }

    QImage *getSpecular() const {
        return specular;
    }

    QImage* getEmission() const {
        return emission;
    }

    QList<Face>& getFaces() {return objModel->getFaces(); }
    const QMatrix4x4& getWorldMatrix() {return worldMatrix;}
    const ObjModel* obj() {return objModel;}

    ~Model3D() {
        delete objModel;
        delete diffuse;
        delete normal;
        delete specular;
        delete emission;
    }
};


#endif //GLPAINTER_MODEL3D_H
