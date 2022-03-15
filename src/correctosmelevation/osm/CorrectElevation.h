// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_CORRECTELEVATION_H_
#define SRC_CORRECTOSMELEVATION_OSM_CORRECTELEVATION_H_

#include <memory>
#include <string>
#include <set>
#include <vector>
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "util/osm/OsmStats.h"

namespace correctosmelevation {
namespace osm {

using util::osm::OsmStats;
using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Work off all route relations and route ways in ranges.
 */
class CorrectElevation {
 public:
  CorrectElevation(const std::string inFile, const std::string outFile,
                   const std::string elevationTag, const uint64_t waysPerRange,
                   const uint64_t relationsPerRange);

  // Collect data about the input file and
  // initialize the average elevation index.
  void initialize();

  // Access to the average elevation index for tests.
  int16_t getElevation(const uint64_t id) const;

  // Correct all route relations in ranges first. Then, correct
  // all remaining route ways in ranges.
  void correctRoutes() const;

  // Collect all route relations in ranges. Return a set of all
  // way ids that were used in the route relations.
  std::set<uint64_t> correctRouteRelationsInRanges(
      const uint64_t absoluteMax, const uint64_t maxPerLoop) const;

  // Collect all way relations in ranges. Exclude ways that
  // were already corrected in route relations.
  void correctRouteWaysInRanges(const std::set<uint64_t>& correctedWayIds,
                                const uint64_t absoluteMax,
                                const uint64_t maxPerLoop) const;

  // Write the updated elevation data to the output OSM file.
  void writeOutputOSM() const;

 private:
  // Procedure to correct all found rivers.
  void correctRivers(NodeIndex& nodeIndex,
                     std::vector<RoutePaths>& rivers) const;

  // Procedure to correct all found tunnels/bridges.
  void correctTunnelsAndBridges(
      NodeIndex& nodeIndex,
      std::vector<RoutePaths>& tunnelsAndBridges) const;

  // Procedure to smooth all found route paths.
  void smoothRoutePaths(NodeIndex& nodeIndex,
                        const std::vector<RoutePaths>& routePaths) const;

  // The average elevation index to store the corrected elevations.
  std::unique_ptr<AverageElevationIndexSparse> _elevationIndex;

  // Statistics about the input OSM file.
  OsmStats _osmStats;

  // Input arguments.
  const std::string _inFile;
  const std::string _outFile;
  const std::string _elevationTag;
  const uint64_t _waysPerRange;
  const uint64_t _relationsPerRange;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_CORRECTELEVATION_H_
