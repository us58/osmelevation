// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_ELEVATIONINDEX_H_
#define SRC_UTIL_INDEX_ELEVATIONINDEX_H_

#include <cstdint>

namespace util {
namespace index {

class ElevationIndex {
 public:
  ElevationIndex(const uint64_t count, const uint64_t max);

  virtual ~ElevationIndex() {}

  // Set the elevation for a node ID.
  virtual void setElevation(const uint64_t nodeId,
                            const int16_t elevation) = 0;

  // Get the elevation for a node ID.
  virtual int16_t getElevation(const uint64_t nodeId) const = 0;

  virtual void process() = 0;

 protected:
  uint64_t _count;
  uint64_t _max;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_ELEVATIONINDEX_H_
