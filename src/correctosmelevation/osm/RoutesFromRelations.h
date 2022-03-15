// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_ROUTESFROMRELATIONS_H_
#define SRC_CORRECTOSMELEVATION_OSM_ROUTESFROMRELATIONS_H_

#include <set>
#include <vector>
#include <cstdint>
#include "correctosmelevation/osm/OsmRoutesRange.h"

namespace correctosmelevation {
namespace osm {

using correctosmelevation::osm::OsmRoutesRange;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Get all route paths, bridges/tunnels, and rivers from OSM
 * relations in a routes range using the OsmRelationsManager.
 */
class RoutesFromRelations : public OsmRoutesRange {
 public:
  using OsmRoutesRange::OsmRoutesRange;

  // Get all route paths, bridges/tunnels, and rivers from OSM
  // relations in a routes range using the OsmRelationsManager.
  // Also, get the ids of all used ways in the route relations.
  void getRoutesAndIds(
      std::set<uint64_t>& correctedWayIds,
      std::vector<RoutePaths>& routePaths,
      std::vector<RoutePaths>& rivers,
      std::vector<RoutePaths>& tunnelsAndBridges) const override;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_ROUTESFROMRELATIONS_H_
