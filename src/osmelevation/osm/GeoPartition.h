// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_OSMELEVATION_OSM_GEOPARTITION_H_
#define SRC_OSMELEVATION_OSM_GEOPARTITION_H_

#include <string>
#include <tuple>
#include "osmelevation/elevation/GeoElevation.h"
#include "util/osm/OsmStats.h"
#include "util/index/ElevationIndex.h"

namespace osmelevation {
namespace osm {

using util::index::ElevationIndex;
using util::osm::OsmStats;
using osmelevation::elevation::GeoElevation;
using GeoBoundary = std::tuple<int16_t, int16_t, int16_t, int16_t>;

/*
 * Get the elevation of all nodes inside a geographic partition
 * using the OsmNodesHandler.
 */
class GeoPartition {
 public:
  GeoPartition(ElevationIndex& elevationIndex,
               const OsmStats& osmStats,
               const std::string& inFile,
               const std::string& nasademDir,
               const GeoBoundary& boundary);

  // Get the elevation of all nodes inside a geographic partition
  // using the OsmNodesHandler.
  void elevationsInPartition();

 private:
  // The index where the elevation data for nodes is stored.
  ElevationIndex& _elevationIndex;

  // Statistics about the input OSM file.
  const OsmStats& _osmStats;

  // The input OSM file.
  const std::string& _inFile;

  // The directory where all NASADEM files are located.
  const std::string& _nasademDir;

  // The boundaries of the geographic partition.
  const GeoBoundary& _boundary;
};

}  // namespace osm
}  // namespace osmelevation

#endif  // SRC_OSMELEVATION_OSM_GEOPARTITION_H_
