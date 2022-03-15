// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_OSM_WAY_H_
#define SRC_UTIL_OSM_WAY_H_

#include <cstdint>

namespace util {
namespace osm {

/*
 * Store an osm way as the id of the first node, the id of
 * the last node and the id of the way itself.
 */
struct Way {
 public:
  Way(uint64_t startId, uint64_t endId, uint64_t id) :
       startId(startId), endId(endId), id(id) {}

  // The id of the first node of the ways' node ref list.
  uint64_t startId;

  // The id of the last node of the way's node ref list.
  uint64_t endId;

  // The id of the way.
  uint64_t id;
};

}  // namespace osm
}  // namespace util

#endif  // SRC_UTIL_OSM_WAY_H_
