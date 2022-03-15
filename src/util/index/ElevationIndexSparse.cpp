// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <math.h>
#include <algorithm>
#include <iostream>
#include "global/Constants.h"
#include "util/index/IdElevationAverage.h"
#include "util/index/ElevationIndex.h"
#include "util/index/ElevationIndexSparse.h"

using global::INVALID_ELEV;
using util::index::IdElevationAverage;
using util::index::ElevationIndex;
using util::index::ElevationIndexSparse;

// ____________________________________________________________________________
ElevationIndexSparse::ElevationIndexSparse(const uint64_t count,
    const uint64_t max) : ElevationIndex(count, max) {
  _sparseIndex.reserve(_count + 1);
  _sparseIndex.emplace_back(0, INVALID_ELEV);
}

// ____________________________________________________________________________
void ElevationIndexSparse::setElevation(const uint64_t nodeId,
                                        const int16_t elevation) {
  if (elevation == INVALID_ELEV) {
    return;
  }
  _sparseIndex.emplace_back(nodeId, elevation);
}

// ____________________________________________________________________________
int16_t ElevationIndexSparse::getElevation(const uint64_t nodeId) const {
  // Binary search.
  int64_t index = 0;
  int64_t left = 0;
  int64_t right = _count - 1;
  while (left <= right) {
    uint64_t m = floor((left + right) / 2);
    if (_sparseIndex[m].id < nodeId) {
      left = m + 1;
    } else if (
      _sparseIndex[m].id > nodeId) {
      right = m - 1;
    } else {
      index = m;
      break;
    }
  }
  const auto& idElev = _sparseIndex[index];
  return (idElev.id == nodeId) ? idElev.elevation : INVALID_ELEV;
}

// ____________________________________________________________________________
void ElevationIndexSparse::process() {
  time_t start, end;
  start = time(&start);
  std::cout << "Sorting the sparse index by node ID." << std::endl;
  // Sort the array by node ID.
  std::sort(_sparseIndex.begin(), _sparseIndex.end(),
            [](const auto& i, const auto& j) {
              return i.id <= j.id;
            });
  _count = _sparseIndex.size();

  // Remove duplicates.
  _sparseIndex.erase(std::unique(_sparseIndex.begin(),
                                 _sparseIndex.end(),
                                 [](const auto& i, const auto& j) {
                                   return i.id == j.id;
                                  }), _sparseIndex.end());

  end = time(&end);
  double timeDiff = difftime(end, start);
  std::cout << "Done, sorting took ";
  std::cout << timeDiff;
  std::cout << " seconds." << "\n" << std::endl;
}
