// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <iostream>
#include <vector>
#include "global/Constants.h"
#include "util/geo/Point.h"
#include "util/geo/Geo.h"
#include "util/osm/Node.h"
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"

using correctosmelevation::correct::CorrectRoute;
using global::INVALID_ELEV;
using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using util::geo::haversine;
using util::osm::Node;
using Coordinate = util::geo::Point<double>;

// _____________________________________________________________________________
CorrectRoute::CorrectRoute(const std::vector<std::vector<uint64_t>>& nodeIds,
                           const std::vector<size_t>& nodeCounts,
                           NodeIndex& nodeIndex,
                           AverageElevationIndexSparse& elevationIndex) :
                           _nodeIds(nodeIds),
                           _nodeCounts(nodeCounts),
                           _nodeIndex(nodeIndex),
                           _elevationIndex(elevationIndex) {
  _allDataAvailable.resize(nodeCounts.size(), true);
}

// _____________________________________________________________________________
void CorrectRoute::buildCoords() {
  for (size_t subroute = 0; subroute < _nodeIds.size(); ++subroute) {
    for (size_t nodeId = 0; nodeId < _nodeCounts[subroute]; ++nodeId) {
      const Node& node = _nodeIndex.getNode(_nodeIds[subroute][nodeId]);
      _nodeCoords[subroute].emplace_back(node.lon, node.lat);
    }
  }
}

// _____________________________________________________________________________
void CorrectRoute::buildElevations() {
  for (size_t subroute = 0; subroute < _nodeIds.size(); ++subroute) {
    for (size_t nodeId = 0; nodeId < _nodeCounts[subroute]; ++nodeId) {
      const Node& node = _nodeIndex.getNode(_nodeIds[subroute][nodeId]);
      _elevations[subroute].emplace_back(node.elevation);
      if (node.elevation == INVALID_ELEV) {
        _allDataAvailable[subroute] = false;
      }
    }
  }
}

// _____________________________________________________________________________
void CorrectRoute::buildDistances() {
  for (size_t subroute = 0; subroute < _nodeIds.size(); ++subroute) {
    double distanceFromStart = 0;
    _distances[subroute].emplace_back(distanceFromStart);
    for (size_t i = 1; i < _nodeCounts[subroute]; ++i) {
      double prevDistance = haversine(_nodeCoords[subroute][i - 1],
                                      _nodeCoords[subroute][i]);
      distanceFromStart += prevDistance;
      _distances[subroute].emplace_back(distanceFromStart);
    }
  }
}

// _____________________________________________________________________________
void CorrectRoute::buildCoordsAndElevations() {
  for (size_t subroute = 0; subroute < _nodeIds.size(); ++subroute) {
    for (size_t nodeId = 0; nodeId < _nodeCounts[subroute]; ++nodeId) {
      const Node& node = _nodeIndex.getNode(_nodeIds[subroute][nodeId]);
      _nodeCoords[subroute].emplace_back(node.lon, node.lat);
      // std::cout << node.lon << ", " << node.lat << std::endl;
      _elevations[subroute].emplace_back(node.elevation);
      if (node.elevation == INVALID_ELEV) {
        _allDataAvailable[subroute] = false;
      }
    }
  }
  // std::cout << "\n\n";
}
