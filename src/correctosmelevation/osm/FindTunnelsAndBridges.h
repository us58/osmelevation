// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_FINDTUNNELSANDBRIDGES_H_
#define SRC_CORRECTOSMELEVATION_OSM_FINDTUNNELSANDBRIDGES_H_

#include <cstdint>
#include <vector>
#include <utility>
#include <osmium/relations/members_database.hpp>
#include <osmium/osm/way.hpp>

namespace correctosmelevation {
namespace osm {

using osmium::relations::MembersDatabase;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Given a path represented by ways, look at the tags
 * of the ways and find valid tunnels and bridges.
 */
class FindTunnelsAndBridges {
 public:
  FindTunnelsAndBridges(
      const MembersDatabase<osmium::Way>& wayDatabase,
      const std::vector<std::pair<uint64_t, uint64_t>>& pathWays);

  // Iterate over each way and find and return
  // all found valid tunnels and bridges.
  std::vector<RoutePaths> getTunnelsAndBridges();

 private:
  // If the beginning of a tunnel or bridge was found, check if valid.
  // If the tunnel or bridge is valid, directly convert the
  // way in front of, the ways representing the tunnel or bridge,
  // and the way after to their 'route paths'. The form is:
  // [route path before, route path of complete tunnel or bridge ways,
  //  route path after]
  size_t newTunnelOrBridge(const size_t start);

  // Check if a way has a key 'embankment' or 'incline'.
  bool embankmentOrIncline(const size_t way) const;

  // Convert a tunnel or bridge way to a route path.
  void addTunnelOrBridgeWay(const size_t PosInTunnelOrBridge,
                            const size_t wayPos);

  // Convert a way before or after a tunnel/bridge to a route path.
  void addWay(const bool before, const size_t wayPos);

  // Database that maps way ids to their data, i.e. the associated
  // nodes and tags. Given by osmium.
  const MembersDatabase<osmium::Way>& _wayDatabase;

  // The path in a route given by ways.
  const std::vector<std::pair<uint64_t, uint64_t>>& _pathWays;

  // The result where each RoutePaths stores a single tunnel or bridge.
  std::vector<RoutePaths> _tunnelsAndBridges;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_FINDTUNNELSANDBRIDGES_H_
