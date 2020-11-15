#ifndef GLPAINTER_ALGORYTHM_H
#define GLPAINTER_ALGORYTHM_H

#include <QtGui/QVector3D>
#include "../obj/ObjModel.h"

QVector3D toBarycentric(const Face &face, float x, float y);
QVector3D reflect(const QVector3D &v, const QVector3D &n);
Vec3i texel(const QImage* image, const QVector3D &pos, const Vec3i& def);

#endif //GLPAINTER_ALGORYTHM_H
