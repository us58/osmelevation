// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_ELEVATIONINDEXDENSE_H_
#define SRC_UTIL_INDEX_ELEVATIONINDEXDENSE_H_

#include <vector>
#include <cstdint>
#include "util/index/ElevationIndex.h"

namespace util {
namespace index {

using util::index::ElevationIndex;

// A 14-bit integer is distributed over 3 consecutive bytes in the dense array.
// There are 4 possibilities of how a 14-bit integer can be layed out
// in the 3 bytes.
// The setBits function as a bit mask to reset the previously stored data in
// the bits of the 14-bit integer.
static const uint8_t setBits[4][3] = { {0b11111100, 0b00000000, 0b00001111},
                                       {0b11110000, 0b00000000, 0b00111111},
                                       {0b11000000, 0b00000000, 0b11111111},
                                       {0b00000000, 0b00000011, 0b11111111} };

// The getBits function as a bit mask to hide the bits that do not belong
// to the 14-bit integer in the 3 bytes.
static const uint8_t getBits[4][3] = { {0b00000011, 0b11111111, 0b11110000},
                                       {0b00001111, 0b11111111, 0b11000000},
                                       {0b00111111, 0b11111111, 0b00000000},
                                       {0b11111111, 0b11111100, 0b00000000} };

/*
 * Store the elevation data per node inside a dense
 * array where the node ID is the index in the array.
 */
class ElevationIndexDense : public ElevationIndex {
 public:
  ElevationIndexDense(const uint64_t count, const uint64_t max);

  // Set the elevation for a node ID.
  void setElevation(const uint64_t nodeId,
                    const int16_t elevation) override;

  // Get the elevation for a node ID.
  int16_t getElevation(const uint64_t nodeId) const override;

  // Nothing has to be done in the dense index.
  void process() override {};

 private:
  // The array that holds the index. The elevation of a node
  // can be directly accessed by the index, as the node id corresponds
  // directly to the index in the array.
  std::vector<int8_t> _denseIndex;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_ELEVATIONINDEXDENSE_H_
