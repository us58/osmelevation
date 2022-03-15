// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include "global/Constants.h"
#include "util/geometry/Vector3d.h"
#include "util/geo/Point.h"
#include "util/geo/Geo.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"
#include "correctosmelevation/correct/CorrectTunnelOrBridge.h"

using global::INVALID_ELEV;
using util::geo::Point;
using util::geometry::Vector3d;
using util::geo::haversineApprox;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectTunnelOrBridge;
using correctosmelevation::correct::CorrectRoute;
using Coordinate = util::geo::Point<double>;

// _____________________________________________________________________________
CorrectTunnelOrBridge::CorrectTunnelOrBridge(
    const std::vector<std::vector<uint64_t>>& nodeIds,
    const std::vector<size_t>& nodeCounts, NodeIndex& nodeIndex,
    AverageElevationIndexSparse& elevationIndex) :
    CorrectRoute(nodeIds, nodeCounts, nodeIndex, elevationIndex) {
  // All sizes are known.
  _elevations.resize(nodeCounts.size());
  _nodeCoords.resize(nodeCounts.size());
  for (size_t subroute = 0; subroute < _nodeCounts.size(); ++subroute) {
    _elevations[subroute].reserve(nodeCounts[subroute]);
    _nodeCoords[subroute].reserve(nodeCounts[subroute]);
  }
  _dataAvailable = true;
}

// _____________________________________________________________________________
void CorrectTunnelOrBridge::createElevationPlane() {
  findNodesBeforeAndAfter(30);

  const Coordinate startPoint = _nodeCoords[0][_startNodeBefore];
  const Coordinate endPoint = _nodeCoords[2][_endNodeAfter];

  const auto startElev = _elevations[0][_startNodeBefore];
  const auto endElev = _elevations[2][_endNodeAfter];

  if (startElev == INVALID_ELEV || endElev == INVALID_ELEV) {
    _dataAvailable = false;
  }

  const auto startVector = Vector3d(startPoint.getX(), startPoint.getY(),
                                    startElev);
  const auto endVector = Vector3d(endPoint.getX(), endPoint.getY(),
                                  endElev);

  const auto directionV1 = endVector - startVector;

  // Parrallel to the XY-plane and normal to startToEndVector.
  const auto directionV2 = directionV1.normalAndParallelToXY();

  _normalVector = directionV1.crossProduct(directionV2);

  _d = -1 * (_normalVector * startVector);
}

// _____________________________________________________________________________
void CorrectTunnelOrBridge::findNodesBeforeAndAfter(const uint16_t dist) {
  // Find the node in the route path in front of the tunnel/bridge,
  // that has a distance of at least <dist> from the beginning of the
  // tunnel/bridge.
  const auto brideTunnelStart = _nodeCoords[1].front();
  size_t startNodeBefore = 1;
  for (size_t i = 1; i < _nodeCounts[0]; ++i) {
    if (haversineApprox(brideTunnelStart, _nodeCoords[0][i]) >= dist) {
      break;
    }
    startNodeBefore = i;
  }
  _startNodeBefore = startNodeBefore;

  // Find the node in the route path after the tunnel/bridge,
  // that has a distance of at least <dist> from the end of the
  // tunnel/bridge.
  const auto bridgeTunnelEnd = _nodeCoords[1].back();
  size_t endNodeAfter = 1;
  for (size_t i = 1; i < _nodeCounts[2]; ++i) {
    if (haversineApprox(bridgeTunnelEnd, _nodeCoords[2][i]) >= dist) {
      break;
    }
    endNodeAfter = i;
  }
  _endNodeAfter = endNodeAfter;
}

// _____________________________________________________________________________
void CorrectTunnelOrBridge::estimateUnknownSection() const {
  if (!_dataAvailable) {
    return;
  }
  for (size_t subroute = 1; subroute < _nodeCounts.size() - 1; ++subroute) {
    for (size_t i = 0; i < _nodeCounts[subroute]; ++i) {
      const auto elev = getElevationOnPlane(_nodeCoords[subroute][i]);
      _elevationIndex.setElevationTunnelOrBridge(
          _nodeIds[subroute][i], elev);
      _nodeIndex.updateElevation(_nodeIds[subroute][i],
                                 static_cast<int16_t>(std::lround(elev)));
    }
  }

  // Also correct the nodes immediate in front of, and after the tunnel/bridge.
  for (size_t i = 1; i < _startNodeBefore + 1; ++i) {
    const auto elev = getElevationOnPlane(_nodeCoords[0][i]);
    _elevationIndex.setElevation(_nodeIds[0][i], elev);
    _nodeIndex.updateElevation(_nodeIds[0][i],
                               static_cast<int16_t>(std::lround(elev)));
  }
  for (size_t i = 1; i < _endNodeAfter + 1; ++i) {
    const auto elev = getElevationOnPlane(_nodeCoords[2][i]);
    _elevationIndex.setElevation(_nodeIds[2][i], elev);
    _nodeIndex.updateElevation(_nodeIds[2][i],
                               static_cast<int16_t>(std::lround(elev)));
  }
}

// _____________________________________________________________________________
float CorrectTunnelOrBridge::getElevationOnPlane(
    const Coordinate& coord) const {
  double elev = (-1 * _d -
                 _normalVector.getX() * coord.getX() -
                 _normalVector.getY() * coord.getY()) / _normalVector.getZ();

  return static_cast<float>(elev);
}
