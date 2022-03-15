// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_OSM_OSMSTATS_H_
#define SRC_UTIL_OSM_OSMSTATS_H_

#include <cstdint>

namespace util {
namespace osm {

struct OsmStats {
  int16_t minLon;
  int16_t minLat;
  int16_t maxLon;
  int16_t maxLat;
  uint64_t nodeCount;
  uint64_t wayCount;
  uint64_t relationCount;
  uint64_t min;
  uint64_t max;
};

}  // namespace osm
}  // namespace util

#endif  // SRC_UTIL_OSM_OSMSTATS_H_
