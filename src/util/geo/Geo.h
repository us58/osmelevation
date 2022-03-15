// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_GEO_GEO_H_
#define SRC_UTIL_GEO_GEO_H_

#include <math.h>

namespace util {
namespace geo {

static const double DEG_RAD = 0.017453292519943295;

// _____________________________________________________________________________
template <typename G1, typename G2>
inline double haversineApprox(const G1& a, const G2& b) {
  const double lon1r = a.getX() * DEG_RAD;
  const double lat1r = a.getY() * DEG_RAD;
  const double lon2r = b.getX() * DEG_RAD;
  const double lat2r = b.getY() * DEG_RAD;

  const double x = (lon2r - lon1r) * cos(0.5 * (lat2r + lat1r));
  const double y = lat2r - lat1r;
  return 6371000.0 * sqrt(x * x + y * y);
}

// _____________________________________________________________________________
template <typename G1, typename G2>
inline double haversine(const G1& a, const G2& b) {
  const double lon1r = a.getX() * DEG_RAD;
  const double lat1r = a.getY() * DEG_RAD;
  const double lon2r = b.getX() * DEG_RAD;
  const double lat2r = b.getY() * DEG_RAD;

  const double u = sin((lat2r - lat1r)/2);
  const double v = sin((lon2r - lon1r)/2);
  return 2.0 * 6371000.0 * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

}  // namespace geo
}  // namespace util

#endif  // SRC_UTIL_GEO_GEO_H_
