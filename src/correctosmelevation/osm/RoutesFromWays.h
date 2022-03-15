// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_ROUTESFROMWAYS_H_
#define SRC_CORRECTOSMELEVATION_OSM_ROUTESFROMWAYS_H_

#include <set>
#include <vector>
#include <cstdint>
#include "correctosmelevation/osm/OsmRoutesRange.h"

namespace correctosmelevation {
namespace osm {

using correctosmelevation::osm::OsmRoutesRange;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Get all route paths from OSM ways in a routes range
 * using the OsmWaysHandler.
 */
class RoutesFromWays : public OsmRoutesRange {
 public:
  using OsmRoutesRange::OsmRoutesRange;

  // Get all route paths from OSM ways in a routes range
  // using the OsmWaysHandler. Exclude route paths that are
  // in the correctedWayIds set.
  std::vector<RoutePaths> getRoutesAndExcludeIds(
      const std::set<uint64_t>& correctedWayIds) const override;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_ROUTESFROMWAYS_H_
