#ifndef GLPAINTER_ALGORYTHM_H
#define GLPAINTER_ALGORYTHM_H

#include <QtGui/QVector3D>
#include "../obj/ObjModel.h"

QVector3D toBarycentric(const Face &face, float x, float y);
QVector3D reflect(const QVector3D &v, const QVector3D &n);

#endif //GLPAINTER_ALGORYTHM_H
