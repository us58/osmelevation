// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_GEOMETRY_VECTOR3D_H_
#define SRC_UTIL_GEOMETRY_VECTOR3D_H_

namespace util {
namespace geometry {

template <typename T>
class Vector3d {
 public:
  Vector3d() : _x(0), _y(0), _z(0) {}
  Vector3d(T x, T y, T z) : _x(x), _y(y), _z(z) {}
  T getX() const { return _x; }
  T getY() const { return _y; }
  T getZ() const { return _z; }

  void setX(T x) { _x = x; }
  void setY(T y) { _y = y; }
  void setZ(T z) { _z = z; }

  bool operator==(const Vector3d<T>& v) const {
    return v.getX() == _x && v.getY() == _y && v.getZ() == _z;
  }

  Vector3d<T> operator-(const Vector3d<T>& v) const {
    return Vector3d<T>(_x - v.getX(), _y - v.getY(), _z - v.getZ());
  }

  T operator*(const Vector3d<T>& v) const {
    return _x * v.getX() + _y * v.getY() + _z * v.getZ();
  }

  Vector3d<T> crossProduct(const Vector3d<T>& v) const {
    return Vector3d<T>(_y * v.getZ() - _z * v.getY(),
                       _z * v.getX() - _x * v.getZ(),
                       _x * v.getY() - _y * v.getX());
  }

  Vector3d<T> normalAndParallelToXY() const {
    return Vector3d<T>(-1 * _y, _x, 0);
  }

 private:
  T _x, _y, _z;
};

}  // namespace geometry
}  // namespace util

#endif  // SRC_UTIL_GEOMETRY_VECTOR3D_H_
