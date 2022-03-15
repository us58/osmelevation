// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_CORRECT_CORRECTTUNNELORBRIDGE_H_
#define SRC_CORRECTOSMELEVATION_CORRECT_CORRECTTUNNELORBRIDGE_H_

#include <vector>
#include <cstdint>
#include "util/index/NodeIndex.h"
#include "util/geometry/Vector3d.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"

namespace correctosmelevation {
namespace correct {

using util::geometry::Vector3d;
using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectRoute;

/*
 * Correct the elevation of a route section that uses a tunnel or bridge.
 * This section is either above or under the ground level which means the
 * real elevation of the section is unknown.
 * By looking at the elevation in front of, and after the section,
 * the unknown elevation can be estimated.
 */
class CorrectTunnelOrBridge : public CorrectRoute {
 public:
  // Expects 3 route paths: The one directly in front of
  // the tunnel/bridge, the one representing the complete tunnel/bridge
  // section, and the one directly after.
  CorrectTunnelOrBridge(const std::vector<std::vector<uint64_t>>& nodeIds,
                        const std::vector<size_t>& nodeCounts,
                        NodeIndex& nodeIndex,
                        AverageElevationIndexSparse& elevationIndex);

  // Create a plane representing the course of elevation of the tunnel/bridge.
  void createElevationPlane();

  // Estimate the elevation of the tunnel/bridge section by sampling from
  // the plane.
  void estimateUnknownSection() const;

 private:
  // Find the nodes in the first and last route path that are <dist>
  // meters away from the tunnel/bridge.
  void findNodesBeforeAndAfter(const uint16_t dist);

  // Get the elevation for a point on the elevation plane.
  float getElevationOnPlane(const Coordinate& coord) const;

  // The normal vector of the plane.
  Vector3d<double> _normalVector;

  // The "d" component of the parametric form of a plane.
  // Calculated by the scalar product of the normal vector and
  // the position-vector of the plane.
  double _d;

  // Denote if the necessary data is available.
  bool _dataAvailable;

  // The nodes in front of, and after the tunnel/bridge section
  // used to span the plane.
  size_t _startNodeBefore;
  size_t _endNodeAfter;
};

}  // namespace correct
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_CORRECT_CORRECTTUNNELORBRIDGE_H_
