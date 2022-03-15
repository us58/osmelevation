// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_IDELEVATIONAVERAGE_H_
#define SRC_UTIL_INDEX_IDELEVATIONAVERAGE_H_

#include <cstdint>

namespace util {
namespace index {

/*
 * Store the average elevation for a node (given by id).
 * Do this by keeping track of the total sum of elevations
 * of the node and a counter of how many elevations contributed
 * to this sum.
 * The average can be extracted by calculating (elevationSum / count).
 */
struct IdElevationAverage {
  IdElevationAverage() {}

  // When creating a new average, initialize count to 1.
  IdElevationAverage(uint64_t id, float elevation) :
                     id(id), elevationSum(elevation), count(1),
                     tunnelOrBridge(false) {}

  // When creating a new average, initialize count to 1.
  IdElevationAverage(uint64_t id, float elevation,
                     bool tunnelOrBridge) :
                     id(id), elevationSum(elevation), count(1),
                     tunnelOrBridge(tunnelOrBridge) {}

  IdElevationAverage(const IdElevationAverage& idEA) :
                     id(idEA.id), elevationSum(idEA.elevationSum),
                     count(idEA.count),
                     tunnelOrBridge(idEA.tunnelOrBridge) {}

  // The id of the node.
  uint64_t id;

  // The total sum of elevations that contribute to the average.
  float elevationSum;

  // The count of how many elevations already contribute to the average.
  uint16_t count;

  // If the node belongs to a tunnel or brigde, no average should be taken
  // and the node will be locked, i.e. the elevations can't be changed again.
  bool tunnelOrBridge;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_IDELEVATIONAVERAGE_H_
