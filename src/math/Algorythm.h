#ifndef GLPAINTER_ALGORYTHM_H
#define GLPAINTER_ALGORYTHM_H

#include <QtGui/QVector3D>
#include "../obj/ObjModel.h"

QVector3D toBarycentric(const Face &face, float x, float y);
QVector3D toBarycentric2(const Face& face, float x, float y);
QVector3D toBarycentric3(const Face& face, float x, float y);
QVector3D toBarycentric2D(const Face& face, float x, float y);
QVector3D reflect(const QVector3D &v, const QVector3D &n);
Vec3i texel(const QImage &image, const QVector3D &pos);

void testCase();

#endif //GLPAINTER_ALGORYTHM_H
