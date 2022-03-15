// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
extern "C" {
#include "../../third_party/utsAlgorithms/sma.h"
}
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"
#include "correctosmelevation/correct/SmoothRoute.h"

using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::SmoothRoute;
using correctosmelevation::correct::CorrectRoute;

// _____________________________________________________________________________
SmoothRoute::SmoothRoute(const std::vector<std::vector<uint64_t>>& nodeIds,
                         const std::vector<size_t>& nodeCounts,
                         NodeIndex& nodeIndex,
                         AverageElevationIndexSparse& elevationIndex) :
                         CorrectRoute(nodeIds, nodeCounts, nodeIndex,
                                      elevationIndex) {
  // All sizes are known.
  _distances.resize(nodeCounts.size());
  _elevations.resize(nodeCounts.size());
  _nodeCoords.resize(nodeCounts.size());
  for (size_t subroute = 0; subroute < _nodeCounts.size(); ++subroute) {
    _distances[subroute].reserve(nodeCounts[subroute]);
    _elevations[subroute].reserve(nodeCounts[subroute]);
    _nodeCoords[subroute].reserve(nodeCounts[subroute]);
  }
}

// _____________________________________________________________________________
void SmoothRoute::smoothMovingAverage(const double windowSize) const {
  // All data needs to be available.
  for (auto allDataAvailable : _allDataAvailable) {
    if (!allDataAvailable) {
      return;
    }
  }
  double halfWindowSize = windowSize / 2.0;

  for (size_t subroute = 0; subroute < _nodeCounts.size(); ++subroute) {
    int nodeCountInt = static_cast<int>(_nodeCounts[subroute]);
    std::vector<double> result;
    result.resize(_nodeCounts[subroute]);

    sma_linear(&_elevations[subroute][0], &_distances[subroute][0],
               &nodeCountInt, &result[0], &halfWindowSize, &halfWindowSize);
    for (size_t i = 0; i < _nodeCounts[subroute]; ++i) {
      _elevationIndex.setElevation(_nodeIds[subroute][i],
                                   static_cast<float>(result[i]));
    }
  }
}
