// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include <utility>
#include <osmium/io/any_input.hpp>
#include <osmium/relations/relations_manager.hpp>
#include "util/osm/Way.h"
#include "correctosmelevation/osm/ProcessRouteRelation.h"
#include "correctosmelevation/osm/OsmRelationsManager.h"

using util::osm::Way;
using correctosmelevation::osm::ProcessRouteRelation;
using correctosmelevation::osm::OsmRelationsManager;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
OsmRelationsManager::OsmRelationsManager(
    std::set<u_int64_t>& correctedWayIds,
    std::vector<RoutePaths>& routePaths,
    std::vector<RoutePaths>& rivers,
    std::vector<RoutePaths>& tunnelsAndBridges,
    const uint64_t rangeStart, const uint64_t rangeEnd) :
    _correctedWayIds(correctedWayIds), _routePaths(routePaths),
    _rivers(rivers), _tunnelsAndBridges(tunnelsAndBridges),
    _rangeStart(rangeStart), _rangeEnd(rangeEnd) {
  _count = 0;
}

// ____________________________________________________________________________
bool OsmRelationsManager::new_relation(const osmium::Relation& relation)
    noexcept {
  bool route = relation.tags().has_tag("type", "route");
  bool river = relation.tags().has_tag("waterway", "river");
  bool needed = route || river;

  if (needed) {
    if (!(_count >= _rangeStart && _count < _rangeEnd)) {
      needed = false;
    }
    ++_count;
  }
  return needed;
}

// ____________________________________________________________________________
bool OsmRelationsManager::new_member(const osmium::Relation& relation,
                                     const osmium::RelationMember& member,
                                     std::size_t n) noexcept {
  (void)relation;
  (void)n;
  bool memberIsWay = member.type() == osmium::item_type::way;
  bool link = !std::strcmp(member.role(), "link");
  bool platform = !std::strcmp(member.role(), "platform");
  return memberIsWay && !link && !platform;
}

// ____________________________________________________________________________
void OsmRelationsManager::complete_relation(const osmium::Relation& relation) {
  if (relation.tags().has_tag("type", "route")) {
    route(relation);
  } else if (relation.tags().has_tag("waterway", "river")) {
    waterway(relation);
  }
}

// ____________________________________________________________________________
void OsmRelationsManager::route(const osmium::Relation& relation) {
  ProcessRouteRelation routeRelation(this->member_ways_database(), relation);
  routeRelation.categorizeWays();
  routeRelation.buildRouteGraph();

  const uint64_t startNode = routeRelation.getRouteStartNodeId();
  const uint64_t endNode = routeRelation.getRouteEndNodeId();

  const auto routePaths = routeRelation.getRoutePaths(startNode, endNode);
  _routePaths.insert(_routePaths.end(),
                     std::make_move_iterator(routePaths.begin()),
                     std::make_move_iterator(routePaths.end()));

  // Update the used way ids.
  const auto correctedWayIds = routeRelation.getUsedWaysIds();
  _correctedWayIds.insert(correctedWayIds.begin(), correctedWayIds.end());

  // Get tunnels and bridges found in the route paths.
  const auto tunnelsAndBridges = routeRelation.getTunnelsAndBridges();
  _tunnelsAndBridges.insert(_tunnelsAndBridges.end(),
                            std::make_move_iterator(tunnelsAndBridges.begin()),
                            std::make_move_iterator(tunnelsAndBridges.end()));
}

// ____________________________________________________________________________
void OsmRelationsManager::waterway(const osmium::Relation& relation) {
  ProcessRouteRelation routeRelation(this->member_ways_database(), relation);
  routeRelation.categorizeWays();
  routeRelation.buildRouteGraph();
  RoutePaths river;

  const uint64_t startNode = routeRelation.getRouteStartNodeId();

  const auto mainStream =
      routeRelation.getRoutePathsFromStartnodes(startNode, 0);

  // Invalid river.
  if (mainStream.size() == 0) {
    return;
  }

  river.insert(river.end(),
               std::make_move_iterator(mainStream.begin()),
               std::make_move_iterator(mainStream.end()));

  const auto sideStreams = routeRelation.getRoutePathsFromUnfinished();
  river.insert(river.end(),
               std::make_move_iterator(sideStreams.begin()),
               std::make_move_iterator(sideStreams.end()));

  _rivers.emplace_back(river);
}
