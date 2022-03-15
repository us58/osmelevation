// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_GEO_POINT_H_
#define SRC_UTIL_GEO_POINT_H_

#include <math.h>
#include <cstdint>

namespace util {
namespace geo {

template <typename T>
class Point {
 public:
  Point(T x, T y) : _x(x), _y(y) {}
  Point(const Point<T>& p) : _x(p.getX()), _y(p.getY()) {}
  T getX() const { return _x; }
  T getY() const { return _y; }

  void setX(T x) { _x = x; }
  void setY(T y) { _y = y; }

  Point<T> operator+(const Point<T>& p) const {
    return Point<T>(_x + p.getX(), _y + p.getY());
  }

  Point<T> operator*(const Point<T>& p) const {
    return Point<T>(_x * p.getX(), _y * p.getY());
  }

  bool operator==(const Point<T>& p) const {
    return p.getX() == _x && p.getY() == _y;
  }

  Point<int16_t> toFloor16() const {
    return Point<int16_t>(floor(_x), floor(_y));
  }

  Point<uint16_t> addOffsetAndNormalize(const Point<int16_t>& offset,
                                        const uint16_t m) const {
    int16_t x = static_cast<int16_t>(_x) + offset.getX();
    x = (x % m + m) % m;

    int16_t y = static_cast<int16_t>(_y) + offset.getY();
    y = (y % m + m) % m;

    return Point<uint16_t>(x, y);
  }

  void addOffsetAndNormalizeInPlace(const Point<int16_t>& offset,
                                    const uint16_t m) {
    int16_t x = static_cast<int16_t>(_x) + offset.getX();
    _x = (x % m + m) % m;

    int16_t y = static_cast<int16_t>(_y) + offset.getY();
    _y = (y % m + m) % m;
  }

 private:
  T _x, _y;
};

}  // namespace geo
}  // namespace util

#endif  // SRC_UTIL_GEO_POINT_H_
