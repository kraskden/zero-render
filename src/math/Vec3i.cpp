#include "Vec3i.h"

Vec3i::Vec3i(int x, int y, int z) : x(x), y(y), z(z) {}

Vec3i operator*(const Vec3i &v, int f) {
    return {v.x * f, v.y * f, v.z * f};
}

QVector3D operator*(const Vec3i &v, float f) {
    return {v.x * f, v.y * f, v.z * f};
}

QVector3D operator/(const Vec3i &v, float f) {
    return {v.x / f, v.y / f, v.z / f};
}
