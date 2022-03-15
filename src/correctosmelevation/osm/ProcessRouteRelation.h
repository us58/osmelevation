// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_PROCESSROUTERELATION_H_
#define SRC_CORRECTOSMELEVATION_OSM_PROCESSROUTERELATION_H_

#include <cstdint>
#include <vector>
#include <set>
#include <utility>
#include <osmium/osm/relation.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/relations/members_database.hpp>
#include "util/osm/Way.h"
#include "util/graph/Graph.h"

namespace correctosmelevation {
namespace osm {

using util::osm::Way;
using util::graph::Graph;
using osmium::relations::MembersDatabase;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Given a route relation and the referenced member ways,
 * extract route paths and tunnels/bridges from the relation.
 */
class ProcessRouteRelation {
 public:
  ProcessRouteRelation(const MembersDatabase<osmium::Way>& wayDatabase,
                       const osmium::Relation& relation);

  // Get all ways with role "forward".
  std::vector<Way> getForward() const;

  // Get all ways with role "backward".
  std::vector<Way> getBackward() const;

  // Get all ways with no direction role specified.
  std::vector<Way> getTwoWay() const;

  // Get all way ids that have been used in a route path.
  // Works only once because the set is moved at return.
  std::set<uint64_t> getUsedWaysIds() const;

  // Get the list of found tunnels/bridges.
  std::vector<RoutePaths> getTunnelsAndBridges();

  // Get the first node of the first way in the relation
  // regarding the correct direction.
  uint64_t getRouteStartNodeId() const;

  // Get the last node of the last way in the relation
  // regarding the correct direction.
  uint64_t getRouteEndNodeId() const;

  // Categorize the members (ways) by their direction,
  // i.e. by forward, backward or two-way if not specified.
  void categorizeWays();

  // Given the forward, backward and two-ways, build a directed
  // graph where each way is an edge and the endpoints of
  // the ways are used as vertices.
  void buildRouteGraph();

  // Convert a path in the directed graph to a route path, i.e.
  // to the individual nodes along the path.
  std::vector<uint64_t> graphPathToRoutePath(
      const std::vector<std::pair<uint64_t, uint64_t>>& path);

  // Get all route paths that were found starting from a startnode.
  std::vector<std::vector<uint64_t>> getRoutePathsFromStartnodes(
      const uint64_t firstNodeId, const uint64_t lastNodeId);

  // Get all route paths that were found in unfinished paths.
  std::vector<std::vector<uint64_t>> getRoutePathsFromUnfinished();

  // Get all unique route paths that were found in the route relation.
  std::vector<RoutePaths> getRoutePaths(const uint64_t firstNodeId,
                                        const uint64_t lastNodeId);

 private:
  // Get a path in the route graph by starting to traverse from startNodeId.
  std::vector<std::pair<uint64_t, uint64_t>> pathFromStartNode(
      const uint64_t startNodeId,
      const std::vector<std::pair<uint64_t, uint64_t>>& unfinishedPath,
      const bool fromStartPoint);

  // Given a path, find and get all valid tunnels/bridges.
  void collectTunnelsAndBridges(
      const std::vector<std::pair<uint64_t, uint64_t>>& path);

  // Database that maps way ids to their data, i.e. the associated
  // nodes and tags. Given by osmium.
  const MembersDatabase<osmium::Way>& _wayDatabase;

  // The relation as given from osmium. Unnecessary members
  // were already removed.
  const osmium::Relation& _relation;

  // Ways specified with the role "forward".
  std::vector<Way> _forward;

  // Ways specified with the role "backward".
  std::vector<Way> _backward;

  // Ways not specified with either forward or backward.
  std::vector<Way> _twoWay;

  // Denote if the route relation is a river or not.
  bool _river;

  // The directed graph of the relation's way.
  Graph _routeGraph;

  // All way ids that were part of the route, i.e. they were
  // been corrected.
  std::set<uint64_t> _usedWaysIds;

  // Tunnels/bridges that were found by the collectTunnelsAndBridges function.
  std::vector<RoutePaths> _tunnelsAndBridges;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_PROCESSROUTERELATION_H_
