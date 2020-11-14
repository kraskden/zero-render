#ifndef GLPAINTER_CONST_H
#define GLPAINTER_CONST_H

#include <cmath>

#define UPDATE_TIMER_TIMEOUT 16

#define SPECULAR_POWER 32

#define AMBIENT_WEIGHT 0.1
#define DIFFUSE_WEIGHT 0.9
#define SPECULAR_WEIGHT 0.1

#define MAX_COLOR 255

#define Z_NEAR 0.4
#define Z_FAR 1000.0
#define DEPTH 200000
#define FOV (M_PI / 4)

#define DEF_MOVE_SPEED 0.21
#define DEF_ROTATE_SPEED (M_PI * 2 / 150.0)
#define DEF_CAMERA_Z 7
#define DEF_CAMERA_YAW (-M_PI / 2)
#define DEF_CAMERA_PITCH 0

#define DELTA_MOVE_SPEED 0.01
#define DELTA_ROTATE_SPEED (M_PI / 400.0)

#define LIGHT_RADIUS 10


#endif //GLPAINTER_CONST_H
