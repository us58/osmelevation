// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_OSMELEVATION_ELEVATION_GEOELEVATION_H_
#define SRC_OSMELEVATION_ELEVATION_GEOELEVATION_H_

#include <string>
#include <unordered_map>
#include "osmelevation/elevation/NasademFile.h"
#include "util/geo/Point.h"

using util::geo::Point;
using CoordInt = util::geo::Point<int16_t>;
using Coordinate = util::geo::Point<double>;

namespace osmelevation {
namespace elevation {

// Horizontally, vertically, and diagonally one cell offset.
static const Point<int16_t> cellOffsets[8] = { Point<int16_t>(0, 1),
                                               Point<int16_t>(1, 1),
                                               Point<int16_t>(1, 0),
                                               Point<int16_t>(1, -1),
                                               Point<int16_t>(0, -1),
                                               Point<int16_t>(-1, -1),
                                               Point<int16_t>(-1, 0),
                                               Point<int16_t>(-1, 1) };

/*
 * Handles getting the elevation from NASADEM files for a coordinate.
 * Each NASADEM has be loaded into memory at least once.
 * After that, keep in memory for fast lookup.
 * The final elevation for a coordinate is interpolated by using
 * additionally incorporating the elevation of the surrounding cells.
 */
class GeoElevation {
 public:
  explicit GeoElevation(const std::string& nasademDir);

  // Get the elevation for a coordinate without any further processing.
  int16_t getElevation(const Coordinate& coord);

  // Get the elevation for a coordinate and interpolate it with the
  // surrounding available cells of the coordinate.
  int16_t getInterpolatedElevation(const Coordinate& coord);

  // Get access to the requested NASADEM file. Either invoke loading
  // the NASADEM file into memory and creating an NasademFile object
  // which provides an interface accessing it, or access the object
  // directly from memory if already done so.
  NasademFile& getNasademFile(const CoordInt& originCoord);

  // Remove all NASADEM files that have been loaded into memory.
  void clear();

 private:
  // The directory where the NASADEM files are located.
  const std::string _nasademDir;

  // The in-memory NASADEM files.
  std::unordered_map<size_t, NasademFile> _nasademFiles;

  // Check if a cell is still inside the same NASADEM file
  // as the origin cell after an offset was added to it.
  static bool cellInNasademFile(const Cell& originCell,
                                const Cell& newcell,
                                const Point<int16_t>& newOffset);

  // Create an unique ID for a coordinate to use the coordinate
  // as a key for an unordered_map.
  static size_t coordToKey(const Point<int16_t>& point);
};

}  // namespace elevation
}  // namespace osmelevation

#endif  // SRC_OSMELEVATION_ELEVATION_GEOELEVATION_H_
