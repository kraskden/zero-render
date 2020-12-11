#ifndef GLPAINTER_PARSER_H
#define GLPAINTER_PARSER_H

#include "ObjModel.h"
#include "Model3D.h"

ObjModel *parseObjFile(const QString &path, MtlContext &mtlContext);

#endif //GLPAINTER_PARSER_H
