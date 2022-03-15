// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_CORRECT_CORRECTRIVER_H_
#define SRC_CORRECTOSMELEVATION_CORRECT_CORRECTRIVER_H_

#include <vector>
#include <cstdint>
#include <set>
#include <unordered_map>
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRoute.h"

namespace correctosmelevation {
namespace correct {

using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectRoute;

/*
 * Store the elevation and all route paths/streams the node
 * is part of in the river.
 */
struct RiverNode {
  explicit RiverNode(int16_t elevation) : elevation(elevation) {}
  int16_t elevation;
  std::set<size_t> routePaths;
};

/*
 * Correct the elevations of nodes that belong to a river
 * on the basis that rivers can not flow uphill.
 */
class CorrectRiver : public CorrectRoute {
 public:
  CorrectRiver(const std::vector<std::vector<uint64_t>>& nodeIds,
               const std::vector<size_t>& nodeCounts, NodeIndex& nodeIndex,
               AverageElevationIndexSparse& elevationIndex);

  // For all nodes in the route paths, build the corresponding river nodes.
  void buildRiverNodes();

  // Rivers can not flow uphill. Check if this property
  // applies to the given river and correct if not.
  void correctDownstreamElevation();

  // Correct all streams/route paths.
  void correctStreams();

  // Correct streams/route paths from the _correctAgain set.
  void mergeStreams();

  // Correct a single route path/stream.
  void correctStream(const size_t subroute);

  // Update the elevation index.
  void updateElevationIndex() const;

 private:
  // Map node ids to their RiverNode object.
  std::unordered_map<uint64_t, RiverNode> _riverNodes;

  // Route paths/streams that need to be checked again.
  std::set<size_t> _correctAgain;
};

}  // namespace correct
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_CORRECT_CORRECTRIVER_H_
