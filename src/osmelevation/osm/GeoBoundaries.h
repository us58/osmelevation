// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_OSMELEVATION_OSM_GEOBOUNDARIES_H_
#define SRC_OSMELEVATION_OSM_GEOBOUNDARIES_H_

#include <cstdint>
#include <tuple>
#include <vector>
#include "util/osm/OsmStats.h"

namespace osmelevation {
namespace osm {

using util::osm::OsmStats;
using GeoBoundary = std::tuple<int16_t, int16_t, int16_t, int16_t>;

/*
 * Divide the given OSM boundary (given by its min and max coordinates)
 * into quadratic tiles with side length tileSize.
 * Each tile is defined by its bottom-left and top-right coordinate.
 */
class GeoBoundaries {
 public:
  GeoBoundaries(const uint8_t size, const OsmStats& osmStats);

  // Return a vector containing the bottom-left and top-right
  // coordinates of each tile. Must be at least one tile.
  std::vector<GeoBoundary> buildBoundaries() const;

 private:
  // Side length of each quadratic tile.
  const uint8_t _size;

  // Stores the OSM boundary.
  const OsmStats& _osmStats;
};

}  // namespace osm
}  // namespace osmelevation

#endif  // SRC_OSMELEVATION_OSM_GEOBOUNDARIES_H_
