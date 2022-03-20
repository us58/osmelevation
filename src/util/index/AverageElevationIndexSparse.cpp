// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <math.h>
#include <ctime>
#include <algorithm>
#include "global/Constants.h"
#include "util/index/IdElevationAverage.h"
#include "util/index/ElevationIndex.h"
#include "util/index/AverageElevationIndexSparse.h"

using global::INVALID_ELEV;
using global::INVALID_ELEV_F;
using util::index::ElevationIndex;
using util::index::AverageElevationIndexSparse;

// ____________________________________________________________________________
AverageElevationIndexSparse::AverageElevationIndexSparse(
    const uint64_t count, const uint64_t max) :
    ElevationIndex(count, max) {
  _averageElevationIndex.reserve(_count + 1);
  _averageElevationIndex.emplace_back(0, INVALID_ELEV);
}

// ____________________________________________________________________________
void AverageElevationIndexSparse::setElevation(
    const uint64_t nodeId, const int16_t elevation) {
  if (elevation == INVALID_ELEV) {
    return;
  }
  _averageElevationIndex.emplace_back(nodeId, static_cast<float>(elevation));
}

// ____________________________________________________________________________
void AverageElevationIndexSparse::setElevation(
    const uint64_t nodeId, const float elevation) {
  if (elevation == INVALID_ELEV_F) {
    return;
  }
  _averageElevationIndex.emplace_back(nodeId, elevation);
}

// ____________________________________________________________________________
void AverageElevationIndexSparse::setElevationTunnelOrBridge(
    const uint64_t nodeId, const float elevation) {
  if (elevation == INVALID_ELEV) {
    return;
  }
  _averageElevationIndex.emplace_back(nodeId, elevation, true);
}

// ____________________________________________________________________________
int16_t AverageElevationIndexSparse::getElevation(
    const uint64_t nodeId) const {
  // Binary search.
  int64_t index = 0;
  int64_t left = 0;
  int64_t right = _count - 1;
  while (left <= right) {
    uint64_t m = floor((left + right) / 2);
    if (_averageElevationIndex[m].id < nodeId) {
      left = m + 1;
    } else if (
      _averageElevationIndex[m].id > nodeId) {
      right = m - 1;
    } else {
      index = m;
      break;
    }
  }
  const auto& idAvgElev = _averageElevationIndex[index];
  if (idAvgElev.count == 0) {
    return INVALID_ELEV;
  }
  return (idAvgElev.id == nodeId) ?
          static_cast<int16_t>(std::lround(idAvgElev.elevationSum /
                                           idAvgElev.count)) : INVALID_ELEV;
}

// ____________________________________________________________________________
void AverageElevationIndexSparse::process() {
  // Sort the array by node ID.
  std::sort(_averageElevationIndex.begin(),
            _averageElevationIndex.end(),
            [](const auto& i, const auto& j) {
              return i.id >= j.id;
            });
  mergeDuplicates();
  _count = _averageElevationIndex.size();
}

// ____________________________________________________________________________
void AverageElevationIndexSparse::mergeDuplicates() {
  // Merge duplicates.
  std::vector<IdElevationAverage> tmpIndex;
  tmpIndex.emplace_back(_averageElevationIndex.back());

  _averageElevationIndex.pop_back();
  while (_averageElevationIndex.size() > 0) {
    const auto& prevElem = _averageElevationIndex.back();
    auto& currentElem = tmpIndex.back();

    // The new average gets merged into the already existing one.
    if (prevElem.id == currentElem.id) {
      // If no tunnel/bridge involved, just add the values.
      if (!prevElem.tunnelOrBridge && !currentElem.tunnelOrBridge) {
        currentElem.elevationSum += prevElem.elevationSum;
        currentElem.count += prevElem.count;
      } else if (prevElem.tunnelOrBridge) {
        currentElem.elevationSum = prevElem.elevationSum;
        currentElem.tunnelOrBridge = true;
        currentElem.count = 1;
      }
    } else {
      tmpIndex.emplace_back(prevElem);
    }
    _averageElevationIndex.pop_back();
  }
  // Move the index back from the temporary vector.
  _averageElevationIndex.swap(tmpIndex);
}
