// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_CORRECT_CORRECTROUTE_H_
#define SRC_CORRECTOSMELEVATION_CORRECT_CORRECTROUTE_H_

#include <cstdint>
#include <vector>
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "util/geo/Point.h"

namespace correctosmelevation {
namespace correct {

using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using Coordinate = util::geo::Point<double>;

/*
 * Build data that is needed to perform corrections on routes.
 */
class CorrectRoute {
 public:
  CorrectRoute(const std::vector<std::vector<uint64_t>>& nodeIds,
               const std::vector<size_t>& nodeCounts,
               NodeIndex& nodeIndex,
               AverageElevationIndexSparse& elevationIndex);

  // Build the coordinates of the nodes, retrieved from the NodeIndex.
  void buildCoords();

  // Build the elevations of the nodes, retrieved from the NodeIndex.
  void buildElevations();

  // Calculate the distance each node has to
  // the first node of its route path.
  void buildDistances();

  // Combine buildCoords() and buildElevations().
  void buildCoordsAndElevations();

 protected:
  // One or more route paths given by the node ids along
  // the route paths.
  const std::vector<std::vector<uint64_t>>& _nodeIds;

  // The number of nodes each route path consists of.
  const std::vector<size_t>& _nodeCounts;

  // The node index where the coordinate and elevation
  // of each node can be retrieved with the node's id.
  NodeIndex& _nodeIndex;

  // Index where the results of the corrections can be stored.
  AverageElevationIndexSparse& _elevationIndex;

  // The distance each node has to the first node of its route path.
  std::vector<std::vector<double>> _distances;

  // The elevation of each node.
  std::vector<std::vector<double>> _elevations;

  // The coordinate of each node.
  std::vector<std::vector<Coordinate>> _nodeCoords;

  // Stores if all data was available for each route path.
  std::vector<bool> _allDataAvailable;
};

}  // namespace correct
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_CORRECT_CORRECTROUTE_H_
