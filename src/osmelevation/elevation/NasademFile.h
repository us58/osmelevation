// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_OSMELEVATION_ELEVATION_NASADEMFILE_H_
#define SRC_OSMELEVATION_ELEVATION_NASADEMFILE_H_

#include <cstdint>
#include <string>
#include <memory>
#include "util/geo/Point.h"

using CoordInt = util::geo::Point<int16_t>;
using Coordinate = util::geo::Point<double>;
using Cell = util::geo::Point<uint16_t>;

namespace osmelevation {
namespace elevation {

/*
 * Load a NASADEM file into memory and provide an interface
 * to extract the elevation data.
 * The NASADEM file is divided into cells.
 * Elevation data for a coordinate can also be extracted by
 * directly providing the cell the coordinate corresponds to.
 */
class NasademFile {
 public:
  NasademFile(const std::string& nasademDir, const CoordInt& coord);

  int16_t getLon() const;
  int16_t getLat() const;
  uint16_t getSamples() const;
  bool exists() const;

  // Get the elevation for a coordinate.
  int16_t getElevationFromCoord(const Coordinate& coord) const;

  // Calculate the cell from the collumn and
  // row and get the elevation in the cell.
  int16_t getElevationFromCell(const Cell& cell) const;

  // Get the row and collumn a coordinate corresponds to.
  Cell getCellFromCoord(const Coordinate& coord) const;

  // Get the center of a cell given by row and column.
  Coordinate getCellCenter(const Cell& cell) const;

 private:
  // Extract the bytes from the NASADEM file.
  std::unique_ptr<uint8_t[]> getData(const std::string& zippedFile);

  // If the requested NASADEM file doesn't exist or is invalid for
  // another reason, return dummy data that will be recognized
  // as invalid.
  std::unique_ptr<uint8_t[]> getDataInvalid();

  // The raw NASADEM elevation data array.
  std::unique_ptr<uint8_t[]> _data;

  // Number of bytes in the data.
  uint32_t _length;

  // The NASADEM file name according to a coordinate.
  std::string _nasademFileName;

  // The coordinate of the bottom left corner of the NASADEM file.
  int16_t _lon;
  int16_t _lat;

  // The cell size in degrees.
  double _cellSize;

  // Number of samples in x- and y-axis.
  uint16_t _samples;

  // The offset of the cell center to one of the edges.
  double _cellCenterOffset;

  // If the file doesn't exist, there is no elevation data available.
  bool _exists;
};

}  // namespace elevation
}  // namespace osmelevation

#endif  // SRC_OSMELEVATION_ELEVATION_NASADEMFILE_H_
