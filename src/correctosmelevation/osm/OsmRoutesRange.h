// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_OSMROUTESRANGE_H_
#define SRC_CORRECTOSMELEVATION_OSM_OSMROUTESRANGE_H_

#include <set>
#include <vector>
#include <string>
#include <cstdint>
#include "util/osm/OsmStats.h"
#include "util/index/NodeIndex.h"

namespace correctosmelevation {
namespace osm {

using util::index::NodeIndex;
using util::osm::OsmStats;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * For a range [start, end), get the start-th to the
 * (end - 1)-th route given the original ordering
 * of routes in an OSM file.
 */
class OsmRoutesRange {
 public:
  OsmRoutesRange(const std::string& osmFile, const OsmStats& osmStats,
                 const std::string& elevationTag,
                 const uint64_t rangeStart, const uint64_t rangeEnd);

  // Get all route relations in the range. Also, get the way ids
  // in the routes by adding them to the correctedWayIds set.
  virtual void getRoutesAndIds(
      std::set<uint64_t>& correctedWayIds,
      std::vector<RoutePaths>& routePaths,
      std::vector<RoutePaths>& rivers,
      std::vector<RoutePaths>& tunnelsAndBridges) const;

  // Get all route ways in the range. Exclude all ways provided in the
  // correctedWayIds set.
  virtual std::vector<RoutePaths> getRoutesAndExcludeIds(
      const std::set<uint64_t>& correctedWayIds) const;

  // Given route paths and rivers, create a set containing all node ids
  // that are present in the route paths and rivers.
  virtual std::set<uint64_t> requiredNodes(
      const std::vector<RoutePaths>& routePaths,
      const std::vector<RoutePaths>& rivers) const;

  // Build a node index containing all nodes that are present
  // in the nodeIds set.
  virtual NodeIndex buildNodeIndex(const std::set<uint64_t>& nodeIds) const;

 protected:
  // The OSM file where the ways and relations get parsed from.
  const std::string& _osmFile;

  // Basic information about the OSM file like
  // total node, way, relation count, etc.
  const OsmStats& _osmStats;

  // The used elevation tag.
  const std::string& _elevationTags;

  // The start of the range.
  const uint64_t _rangeStart;

  // The end of the range.
  const uint64_t _rangeEnd;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_OSMROUTESRANGE_H_
