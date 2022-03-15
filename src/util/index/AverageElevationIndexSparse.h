// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_AVERAGEELEVATIONINDEXSPARSE_H_
#define SRC_UTIL_INDEX_AVERAGEELEVATIONINDEXSPARSE_H_

#include <vector>
#include <utility>
#include <cstdint>
#include "util/index/IdElevationAverage.h"
#include "util/index/ElevationIndex.h"

namespace util {
namespace index {

using util::index::ElevationIndex;
using util::index::IdElevationAverage;

/*
 * Sparse index to store the average elevation for nodes given by ids. 
 * The averages are maintained by adding duplicates to the index.
 * The duplicates get merged in the process function, with result
 * that after calling process, there is one representation of each
 * node in the index that holds the average.
 */
class AverageElevationIndexSparse : public ElevationIndex {
 public:
  AverageElevationIndexSparse(const uint64_t count, const uint64_t max);

  // Add the elevation for a node, duplicates are allowed.
  void setElevation(const uint64_t nodeId,
                    const int16_t elevation);

  // Add the elevation for a node, duplicates are allowed.
  void setElevation(const uint64_t nodeId,
                    const float elevation);

  // Add the elevation for a node belonging to a tunnel or bridge,
  // duplicates are allowed.
  void setElevationTunnelOrBridge(const uint64_t nodeId,
                                  const float elevation);

  // Get the average elevation for a node.
  int16_t getElevation(const uint64_t nodeId) const override;

  // Sort all entries (nodes) by id, then merge all duplicates (based on id)
  // to maintain the averages of the nodes elevations.
  void process() override;

 private:
  // Remove duplicates by keeping one version of each node which
  // stores the sum of the elevations of all duplicates and the total
  // number of duplicates so far.
  void mergeDuplicates();

  // The array that holds the index. Valid after being sorted by id.
  std::vector<IdElevationAverage> _averageElevationIndex;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_AVERAGEELEVATIONINDEXSPARSE_H_
