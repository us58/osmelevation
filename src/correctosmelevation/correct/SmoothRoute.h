// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_CORRECT_SMOOTHROUTE_H_
#define SRC_CORRECTOSMELEVATION_CORRECT_SMOOTHROUTE_H_

#include <cstdint>
#include <vector>
#include "util/index/NodeIndex.h"
#include "util/geo/Point.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"

namespace correctosmelevation {
namespace correct {

using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectRoute;

/*
 * Smooth a route by averaging the nodes' elevations.
 */
class SmoothRoute : public CorrectRoute {
 public:
  SmoothRoute(const std::vector<std::vector<uint64_t>>& nodeIds,
              const std::vector<size_t>& nodeCounts,
              NodeIndex& nodeIndex,
              AverageElevationIndexSparse& elevationIndex);

  // Apply a linear moving average to each node.
  // See https://github.com/andreas50/utsOperators for details.
  void smoothMovingAverage(const double windowSize) const;
};

}  // namespace correct
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_CORRECT_SMOOTHROUTE_H_
