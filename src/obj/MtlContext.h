#ifndef GLPAINTER_MTLCONTEXT_H
#define GLPAINTER_MTLCONTEXT_H


#include "../common/types.h"

class MtlContext {
protected:
    QSettings* settings;
public:
    virtual Mtl* getDefMtl() = 0;
    virtual void addMtls(const QMap<QString, Mtl*>& data) = 0;
    virtual Mtl* getMtl(const QString& name) = 0;

    const QSettings* getSettings() const {
        return settings;
    }

    virtual bool isTextureSearch() = 0;

    virtual ~MtlContext() {
        delete settings;
    }
};

#endif //GLPAINTER_MTLCONTEXT_H
