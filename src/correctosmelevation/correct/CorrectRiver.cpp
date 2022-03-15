// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"
#include "correctosmelevation/correct/CorrectRiver.h"

using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectRiver;
using correctosmelevation::correct::CorrectRoute;
using correctosmelevation::correct::RiverNode;

// _____________________________________________________________________________
CorrectRiver::CorrectRiver(const std::vector<std::vector<uint64_t>>& nodeIds,
                           const std::vector<size_t>& nodeCounts,
                           NodeIndex& nodeIndex,
                           AverageElevationIndexSparse& elevationIndex) :
                           CorrectRoute(nodeIds, nodeCounts, nodeIndex,
                                        elevationIndex) {
  _elevations.resize(_nodeCounts.size());
  for (size_t subroute = 0; subroute < _nodeCounts.size(); ++subroute) {
    _elevations[subroute].reserve(nodeCounts[subroute]);
  }
}

// _____________________________________________________________________________
void CorrectRiver::buildRiverNodes() {
  // If not all elevation data was available, the river cannot be corrected.
  for (auto allDataAvailable : _allDataAvailable) {
    if (!allDataAvailable) {
      return;
    }
  }
  for (size_t subroute = 0; subroute < _nodeCounts.size(); ++subroute) {
    for (size_t i = 0; i < _nodeCounts[subroute]; ++i) {
      _riverNodes.try_emplace(_nodeIds[subroute][i], _elevations[subroute][i]);
    }
  }
}

// _____________________________________________________________________________
void CorrectRiver::correctDownstreamElevation() {
  if (_riverNodes.size() == 0) {
    return;
  }
  correctStreams();

  uint16_t count = 0;
  while (_correctAgain.size() > 0 && count < 5000) {
    ++count;
    mergeStreams();
  }
}

// _____________________________________________________________________________
void CorrectRiver::correctStreams() {
  for (size_t stream = 0; stream < _nodeCounts.size(); ++stream) {
    correctStream(stream);
  }
}

// _____________________________________________________________________________
void CorrectRiver::mergeStreams() {
  for (size_t subroute = 0; subroute < _nodeCounts.size(); ++subroute) {
    if (_correctAgain.contains(subroute)) {
      _correctAgain.erase(subroute);
      correctStream(subroute);
    }
  }
}

// _____________________________________________________________________________
void CorrectRiver::correctStream(const size_t subroute) {
  for (size_t i = 1; i < _nodeCounts[subroute]; ++i) {
    auto node = _riverNodes.find(_nodeIds[subroute][i]);
    auto prevNode = _riverNodes.find(_nodeIds[subroute][i - 1]);
    bool corrected = false;
    if (node->second.elevation > prevNode->second.elevation) {
      corrected = true;
      node->second.elevation = prevNode->second.elevation;
    }
    node->second.routePaths.emplace(subroute);

    // If the node was corrected and is part of multiple route paths/streams,
    // the other route paths/streams must be checked again later.
    const bool seperateStream = (node->second.routePaths.size() > 1);
    if (seperateStream && corrected) {
      for (const auto& corrected : node->second.routePaths) {
        if (corrected != subroute) {
          _correctAgain.emplace(corrected);
        }
      }
    }
  }
}

// _____________________________________________________________________________
void CorrectRiver::updateElevationIndex() const {
  for (const auto& node : _riverNodes) {
    _elevationIndex.setElevation(node.first, node.second.elevation);
  }
}
