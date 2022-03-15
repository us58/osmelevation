// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_OSMRELATIONSMANAGER_H_
#define SRC_CORRECTOSMELEVATION_OSM_OSMRELATIONSMANAGER_H_

#include <vector>
#include <set>
#include <cstdint>
#include <osmium/relations/relations_manager.hpp>

namespace correctosmelevation {
namespace osm {

using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Collect and process all route relations in a specified routes range.
 * Categorize into route paths, tunnels/bridges, and rivers.
 */
class OsmRelationsManager : public osmium::relations::RelationsManager<OsmRelationsManager, true, true, true> {  // NOLINT
 public:
  OsmRelationsManager(std::set<u_int64_t>& correctedWayIds,
                      std::vector<RoutePaths>& routePaths,
                      std::vector<RoutePaths>& rivers,
                      std::vector<RoutePaths>& tunnelsAndBridges,
                      const uint64_t rangeStart, const uint64_t rangeEnd);

  // Specify which relations to take.
  // This includes relations with tag "type=route" and "waterway=river".
  // Also, the relations must be the specified routes range.
  bool new_relation(const osmium::Relation& relation) noexcept;

  // Specify which members of a relation to take.
  bool new_member(const osmium::Relation& relation,
                  const osmium::RelationMember& member,
                  std::size_t n) noexcept;

  // All data of a relation is available. Invoke processing.
  void complete_relation(const osmium::Relation& relation);

  // Process a route relations.
  void route(const osmium::Relation& relation);

  // Process a river.
  void waterway(const osmium::Relation& relation);

 private:
  // The ids of used ways in the route relations.
  std::set<uint64_t>& _correctedWayIds;

  // All route paths that were found.
  std::vector<RoutePaths>& _routePaths;

  // All rivers that were found.
  std::vector<RoutePaths>& _rivers;

  // All tunnels/bridges that were found.
  std::vector<RoutePaths>& _tunnelsAndBridges;

  // The start of the routes range.
  const uint64_t _rangeStart;

  // The end of the routes range.
  const uint64_t _rangeEnd;

  // Counter to keep track of the routes range.
  uint64_t _count;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_OSMRELATIONSMANAGER_H_
