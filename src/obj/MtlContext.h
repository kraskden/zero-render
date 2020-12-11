#ifndef GLPAINTER_MTLCONTEXT_H
#define GLPAINTER_MTLCONTEXT_H


#include "../common/types.h"

class MtlContext {
public:
    virtual Mtl* getDefMtl() = 0;
    virtual void addMtls(const QMap<QString, Mtl*>& data) = 0;
    virtual Mtl* getMtl(const QString& name) = 0;
};

#endif //GLPAINTER_MTLCONTEXT_H
