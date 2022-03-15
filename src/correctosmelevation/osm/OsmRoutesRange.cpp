// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <set>
#include <vector>
#include <string>
#include <cstdint>
#include "util/index/NodeIndex.h"
#include "util/osm/OsmStats.h"
#include "parser/NodeParser.h"
#include "correctosmelevation/osm/OsmNodesHandler.h"
#include "correctosmelevation/osm/OsmRoutesRange.h"

using correctosmelevation::osm::OsmRoutesRange;
using util::index::NodeIndex;
using util::osm::OsmStats;
using correctosmelevation::osm::OsmNodesHandler;
using parser::NodeParser;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// _____________________________________________________________________________
OsmRoutesRange::OsmRoutesRange(const std::string& osmFile,
                               const OsmStats& osmStats,
                               const std::string& elevationTag,
                               const uint64_t rangeStart,
                               const uint64_t rangeEnd) :
                               _osmFile(osmFile), _osmStats(osmStats),
                               _elevationTags(elevationTag),
                               _rangeStart(rangeStart),
                               _rangeEnd(rangeEnd) {}

// _____________________________________________________________________________
void OsmRoutesRange::getRoutesAndIds(
    std::set<uint64_t>& correctedWayIds,
    std::vector<RoutePaths>& routePaths,
    std::vector<RoutePaths>& rivers,
    std::vector<RoutePaths>& tunnelsAndBridges) const {
    (void)correctedWayIds;
    (void)routePaths;
    (void)rivers;
    (void)tunnelsAndBridges;
}

// _____________________________________________________________________________
std::vector<RoutePaths> OsmRoutesRange::getRoutesAndExcludeIds(
    const std::set<uint64_t>& correctedWayIds) const {
    (void)correctedWayIds;
    return std::vector<RoutePaths>();
}

// _____________________________________________________________________________
std::set<uint64_t> OsmRoutesRange::requiredNodes(
    const std::vector<RoutePaths>& routePaths,
    const std::vector<RoutePaths>& rivers) const {
  // Make set containing all node ids that are needed.
  std::set<uint64_t> uniqueIds;
  for (const auto& routePath : routePaths) {
    for (const auto& subPath : routePath) {
      for (const auto& nodeId : subPath) {
        uniqueIds.emplace(nodeId);
      }
    }
  }
  for (const auto& river : rivers) {
    for (const auto& substream : river) {
      for (const auto& nodeId : substream) {
        uniqueIds.emplace(nodeId);
      }
    }
  }
  return uniqueIds;
}

// _____________________________________________________________________________
NodeIndex OsmRoutesRange::buildNodeIndex(
    const std::set<uint64_t>& nodeIds) const {
  // Build a node index given needed node ids.
  NodeIndex nodeIndex(nodeIds.size());
  OsmNodesHandler osmNodesHandler(nodeIndex, _elevationTags, nodeIds);
  NodeParser nParser(_osmFile, &osmNodesHandler, _osmStats);
  nParser.parseNoProgressBar();
  nodeIndex.sort();

  return nodeIndex;
}
