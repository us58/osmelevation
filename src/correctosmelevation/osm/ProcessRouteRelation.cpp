// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <cstdint>
#include <vector>
#include <set>
#include <iterator>
#include <utility>
#include <osmium/osm/relation.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/relations/members_database.hpp>
#include "util/osm/Way.h"
#include "util/graph/Graph.h"
#include "correctosmelevation/osm/FindTunnelsAndBridges.h"
#include "correctosmelevation/osm/ProcessRouteRelation.h"

using util::osm::Way;
using util::graph::Graph;
using correctosmelevation::osm::FindTunnelsAndBridges;
using correctosmelevation::osm::ProcessRouteRelation;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
ProcessRouteRelation::ProcessRouteRelation(
    const MembersDatabase<osmium::Way>& wayDatabase,
    const osmium::Relation& relation) :
    _wayDatabase(wayDatabase), _relation(relation) {
  _forward.reserve(_relation.members().size());
  _backward.reserve(_relation.members().size());
  _twoWay.reserve(_relation.members().size() * 2);
  _routeGraph = Graph(relation.members().size());

  _river = (relation.tags().has_tag("waterway", "river")) ? true : false;
}

// ____________________________________________________________________________
std::vector<Way> ProcessRouteRelation::getForward() const {
  return _forward;
}

// ____________________________________________________________________________
std::vector<Way> ProcessRouteRelation::getBackward() const {
  return _backward;
}

// ____________________________________________________________________________
std::vector<Way> ProcessRouteRelation::getTwoWay() const {
  return _twoWay;
}

// ____________________________________________________________________________
std::set<uint64_t> ProcessRouteRelation::getUsedWaysIds() const {
  return std::move(_usedWaysIds);
}

// ____________________________________________________________________________
std::vector<RoutePaths> ProcessRouteRelation::getTunnelsAndBridges() {
  return std::move(_tunnelsAndBridges);
}

// ____________________________________________________________________________
uint64_t ProcessRouteRelation::getRouteStartNodeId() const {
  // The start node is located in the first member that is a way.
  uint64_t startNodeId = 0;
  for (const auto& member : _relation.members()) {
    // member.ref() will be 0 for all members we're not interested
    // in. The objects for those members are not available.
    if (member.ref() != 0) {
      if (!std::strcmp(member.role(), "backward")) {
        startNodeId = _wayDatabase.get(member.ref())->nodes().back().ref();
      } else {
        startNodeId = _wayDatabase.get(member.ref())->nodes().front().ref();
      }
      break;
    }
  }
  return startNodeId;
}

// ____________________________________________________________________________
uint64_t ProcessRouteRelation::getRouteEndNodeId() const {
  // The start node is located in the first member that is a way.
  uint64_t endNodeId = 0;
  for (const auto& member : _relation.members()) {
    // member.ref() will be 0 for all members we're not interested
    // in. The objects for those members are not available.
    if (member.ref() != 0) {
      if (!std::strcmp(member.role(), "forward") ||
          !std::strcmp(member.role(), "")) {
        endNodeId = _wayDatabase.get(member.ref())->nodes().front().ref();
      } else {
        endNodeId = _wayDatabase.get(member.ref())->nodes().back().ref();
      }
    }
  }
  return endNodeId;
}

// ____________________________________________________________________________
void ProcessRouteRelation::categorizeWays() {
  // Iterate over all members.
  for (const auto& member : _relation.members()) {
    // member.ref() will be 0 for all members you are not interested
    // in. The objects for those members are not available.
    if (member.ref() != 0) {
      const osmium::Way* way = _wayDatabase.get(member.ref());
      const auto& nodes = way->nodes();
      const bool oneway = way->tags().has_tag("oneway", "yes");
      const bool twoway = way->tags().has_tag("oneway", "no");
      const bool forward = !std::strcmp(member.role(), "forward");
      const bool backward = !std::strcmp(member.role(), "backward");

       // Collect the ways' start- and endpoints and categorize them correctly.
      if (forward || (oneway && !backward) || _river) {
        _forward.emplace_back(nodes.front().ref(),
                              nodes.back().ref(), way->id());
      } else if (backward) {
        _backward.emplace_back(nodes.back().ref(),
                               nodes.front().ref(), way->id());
      } else if (twoway) {
        _twoWay.emplace_back(nodes.front().ref(),
                             nodes.back().ref(), way->id());
      } else {
        _twoWay.emplace_back(nodes.front().ref(),
                             nodes.back().ref(), way->id());
      }
    }
  }
}

// ____________________________________________________________________________
void ProcessRouteRelation::buildRouteGraph() {
  for (const auto& way : _forward) {
    _routeGraph.addEdge(way.startId, way.endId, way.id, false);
  }
  for (const auto& way : _backward) {
    _routeGraph.addEdge(way.startId, way.endId, way.id, false);
  }
  for (const auto& way : _twoWay) {
    _routeGraph.addEdge(way.startId, way.endId, way.id, true);
    _routeGraph.addEdge(way.endId, way.startId, way.id, true);
  }
}

// ____________________________________________________________________________
std::vector<uint64_t> ProcessRouteRelation::graphPathToRoutePath(
    const std::vector<std::pair<uint64_t, uint64_t>>& path) {
  std::vector<uint64_t> routePath;

  // Must at least be one way.
  if (path.size() == 0) {
    return routePath;
  }

  // Add the start-node.
  const auto& firstWayNodes =
      _wayDatabase.get(path.front().second)->nodes();
  if (path.front().first == firstWayNodes.front()) {
    routePath.emplace_back(firstWayNodes.back().ref());
  } else {
    routePath.emplace_back(firstWayNodes.front().ref());
  }

  for (const auto& way : path) {
    _usedWaysIds.emplace(way.second);
    const auto& nodes = _wayDatabase.get(way.second)->nodes();

    // Check if the way is needed forward or backward.
    if (way.first == nodes.front()) {
      for (auto rit = nodes.crbegin() + 1; rit != nodes.crend(); ++rit) {
        routePath.emplace_back(rit->ref());
      }
    } else {
      for (auto rit = nodes.cbegin() + 1; rit != nodes.cend(); ++rit) {
        routePath.emplace_back(rit->ref());
      }
    }
  }
  return routePath;
}

// ____________________________________________________________________________
void ProcessRouteRelation::collectTunnelsAndBridges(
    const std::vector<std::pair<uint64_t, uint64_t>>& path) {
  // Check for tunnels and bridges in the subroute.
  FindTunnelsAndBridges findTunnelsAndBridges(_wayDatabase, path);
  const auto tunnelsAndBridges = findTunnelsAndBridges.getTunnelsAndBridges();
  if (tunnelsAndBridges.size() != 0) {
    _tunnelsAndBridges.insert(
        _tunnelsAndBridges.end(),
        std::make_move_iterator(tunnelsAndBridges.begin()),
        std::make_move_iterator(tunnelsAndBridges.end()));
  }
}

// ____________________________________________________________________________
std::vector<std::pair<uint64_t, uint64_t>>
    ProcessRouteRelation::pathFromStartNode(const uint64_t startNodeId,
        const std::vector<std::pair<uint64_t, uint64_t>>& unfinishedPath,
        const bool fromStartPoint) {
  const auto path = _routeGraph.traverseGraph(startNodeId, unfinishedPath,
                                              fromStartPoint, _river);
  return path;
}

// ____________________________________________________________________________
std::vector<std::vector<uint64_t>>
    ProcessRouteRelation::getRoutePathsFromStartnodes(
        const uint64_t firstNodeId, const uint64_t lastNodeId) {
  _routeGraph.findStartpoints();
  // No unfinished paths if starting from start-nodes.
  const std::vector<std::pair<uint64_t, uint64_t>> unfinishedPath;
  std::vector<std::vector<uint64_t>> subRoutesNodes;

  uint64_t first = (_routeGraph.removeStartNode(firstNodeId)) ? firstNodeId : 0;
  uint64_t last = (_routeGraph.removeStartNode(lastNodeId)) ? lastNodeId : 0;

  // Build route path from the first and last node of the relation.
  if (first != 0) {
    const auto subroute = pathFromStartNode(first, unfinishedPath, true);
    subRoutesNodes.emplace_back(graphPathToRoutePath(subroute));
    if (!_river) { collectTunnelsAndBridges(subroute); }
  } else if (_river) {
    return subRoutesNodes;  // Invalid river
  }
  if (last != 0) {
    const auto subroute = pathFromStartNode(last, unfinishedPath, true);
    subRoutesNodes.emplace_back(graphPathToRoutePath(subroute));
    if (!_river) { collectTunnelsAndBridges(subroute); }
  }

  // Build route paths starting from all other found startpoints.
  uint64_t currentStart = _routeGraph.getStartpoint();
  uint64_t count = 0;
  while (currentStart != 0 && count < 200) {
    ++count;
    const auto subRoute = pathFromStartNode(currentStart, unfinishedPath, true);

    if (subRoute.size() == 0) {
      currentStart = _routeGraph.getStartpoint();
      continue;
    }
    subRoutesNodes.emplace_back(graphPathToRoutePath(subRoute));
    if (!_river) { collectTunnelsAndBridges(subRoute); }
    currentStart = _routeGraph.getStartpoint();
  }
  return subRoutesNodes;
}

// ____________________________________________________________________________
std::vector<std::vector<uint64_t>>
    ProcessRouteRelation::getRoutePathsFromUnfinished() {
  std::vector<std::vector<uint64_t>> routePaths;

  // Build route paths by start building from unfinished paths.
  auto unfinishedPath = _routeGraph.getUnfinishedPath();
  uint64_t count = 0;
  while (unfinishedPath.size() != 0 && count < 10000) {
    ++count;

    // Start building from the last node of the unfinished path.
    auto remainingPath =
        pathFromStartNode(unfinishedPath.back().first, unfinishedPath, false);

    // If the remaining path is empty, the path was already covered.
    if (remainingPath.size() == 0) {
      unfinishedPath = _routeGraph.getUnfinishedPath();
      continue;
    }
    unfinishedPath.insert(unfinishedPath.end(),
                          std::make_move_iterator(remainingPath.begin()),
                          std::make_move_iterator(remainingPath.end()));

    // Convert to route path.
    routePaths.emplace_back(graphPathToRoutePath(unfinishedPath));

    // Find and collect tunnels/bridges in the path.
    if (!_river) { collectTunnelsAndBridges(unfinishedPath); }

    // Get the next unfinished path.
    unfinishedPath = _routeGraph.getUnfinishedPath();
  }
  return routePaths;
}

// ____________________________________________________________________________
std::vector<RoutePaths> ProcessRouteRelation::getRoutePaths(
    const uint64_t firstNodeId, const uint64_t lastNodeId) {
  std::vector<RoutePaths> routePaths;

  const auto subroutesFromStartnodes =
      getRoutePathsFromStartnodes(firstNodeId, lastNodeId);
  for (const auto& subroute : subroutesFromStartnodes) {
    routePaths.emplace_back(RoutePaths());
    routePaths.back().emplace_back(subroute);
  }
  const auto subroutesFromUnfinished = getRoutePathsFromUnfinished();
  for (const auto& subroute : subroutesFromUnfinished) {
    routePaths.emplace_back(RoutePaths());
    routePaths.back().emplace_back(subroute);
  }
  return routePaths;
}
