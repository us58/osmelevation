// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_INDEX_ELEVATIONINDEXSPARSE_H_
#define SRC_UTIL_INDEX_ELEVATIONINDEXSPARSE_H_

#include <vector>
#include <cstdint>
#include "util/index/IdElevation.h"
#include "util/index/ElevationIndex.h"

namespace util {
namespace index {

using util::index::IdElevation;
using util::index::ElevationIndex;

/*
 * Store the elevation information per node inside a sparse
 * array sorted by node id.
 */
class ElevationIndexSparse : public ElevationIndex {
 public:
  ElevationIndexSparse(const uint64_t count, const uint64_t max);

  // Set the elevation for a node ID.
  void setElevation(const uint64_t nodeId,
                    const int16_t elevation) override;

  // Get the elevation for a node ID.
  int16_t getElevation(const uint64_t nodeId) const override;

  // When all nodes were set, sort the index by node ID.
  void process() override;

 private:
  // The array that holds the index. Valid after being sorted by id.
  std::vector<IdElevation> _sparseIndex;
};

}  // namespace index
}  // namespace util

#endif  // SRC_UTIL_INDEX_ELEVATIONINDEXSPARSE_H_
