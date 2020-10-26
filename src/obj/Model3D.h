#ifndef GLPAINTER_MODEL3D_H
#define GLPAINTER_MODEL3D_H


#include <QtGui/QMatrix4x4>
#include "ObjModel.h"

enum ModelType {
    ENTITY, LIGHT_SOURCE
};

class Model3D {
    ObjModel* objModel = nullptr;
    QMatrix4x4 worldMatrix;
    ModelType modelType;
public:
    Model3D(ObjModel* objModel, QMatrix4x4 worldMatrix, ModelType type = ModelType::ENTITY) {
        this->objModel = objModel;
        this->worldMatrix = worldMatrix;
        this->modelType = type;
    }

    QList<Face>& getFaces() {return objModel->getFaces(); }
    const QMatrix4x4& getWorldMatrix() {return worldMatrix;}
    ModelType getType() {return modelType; }
    const ObjModel* obj() {return objModel;}

    ~Model3D() {
        delete objModel;
    }
};


#endif //GLPAINTER_MODEL3D_H
