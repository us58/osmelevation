// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_IDELEVATION_H_
#define SRC_UTIL_INDEX_IDELEVATION_H_

#include <cstdint>

namespace util {
namespace index {

/*
 * Simple structure to store the elevation for a node id.
 */
struct IdElevation {
  IdElevation() {}

  IdElevation(uint64_t id, int16_t elevation) :
              id(id), elevation(elevation) {}

  // The id of the node.
  uint64_t id;

  // The elevation at the node's location.
  int16_t elevation;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_IDELEVATION_H_
