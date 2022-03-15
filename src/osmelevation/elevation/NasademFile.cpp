// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <zip.h>
#include <math.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <tuple>
#include "osmelevation/elevation/NasademFile.h"
#include "util/geo/Point.h"
#include "global/Constants.h"
#include "osmelevation/elevation/NasademFileName.h"

using osmelevation::elevation::NasademFile;
using osmelevation::elevation::convertToNasademNaming;
using osmelevation::elevation::getNasademFilePath;
using CoordInt = util::geo::Point<int16_t>;
using Coordinate = util::geo::Point<double>;
using Cell = util::geo::Point<uint16_t>;
using global::INVALID_ELEV;
using global::MIN_ELEV_EARTH;
using global::MAX_ELEV_EARTH;

// ____________________________________________________________________________
NasademFile::NasademFile(const std::string& nasademDir,
                         const CoordInt& coord) {
  _nasademFileName = convertToNasademNaming(coord);
  std::string fp = getNasademFilePath(nasademDir, _nasademFileName);
  _exists = true ? fp != std::string("invalid") : false;
  _data = getData(fp);
  _samples = floor(sqrt(_length / 2));
  _cellSize = _exists ? 1 / static_cast<double>(_samples - 1) : 1;
  _cellCenterOffset = _cellSize / static_cast<double>(2);
  _lon = floor(coord.getX());
  _lat = floor(coord.getY());
}

// ____________________________________________________________________________
int16_t NasademFile::getLon() const {
  return _lon;
}

// ____________________________________________________________________________
int16_t NasademFile::getLat() const {
  return _lat;
}

// ____________________________________________________________________________
uint16_t NasademFile::getSamples() const {
  return _samples;
}

// ____________________________________________________________________________
bool NasademFile::exists() const {
  return _exists;
}

// ____________________________________________________________________________
std::unique_ptr<uint8_t[]> NasademFile::getData(
    const std::string& zippedFile) {
  // If the NASADEM file doesn't exist, write 10000 into contents.
  if (!_exists) {
    return getDataInvalid();
  }

  // Open the ZIP archive.
  int err = 0;
  zip* z = zip_open(zippedFile.c_str(), 0, &err);
  std::string nasademFileName = _nasademFileName + ".hgt";

  struct zip_stat st;
  zip_stat_init(&st);
  zip_stat(z, nasademFileName.c_str(), 0, &st);

  _length = st.size;  // Length of the uncompressed data.

  // If the length of the uncompressed data is 0, the requested
  // NASADEM file didn't exist inside the zip archive.
  // Also check that the length is a power of 2, otherwise
  // the file is not a valid NASADEM file.
  if (_length == 0 || !(floor(sqrt(_length / 2)) == sqrt(_length / 2))) {
    std::cout << "Invalid NASADEM file detected in zip archive <";
    std::cout << zippedFile << ">." << std::endl;
    std::cout << "The NASADEM file <" << nasademFileName << "> either ";
    std::cout << "does not exist or has an incompatible format." << std::endl;
    std::cout << "Continuing.... " << std::endl;
    return getDataInvalid();
  }

  // Allocate memory for its uncompressed contents.
  auto contents = std::make_unique<uint8_t[]>(_length);

  // Read the compressed file.
  zip_file* f = zip_fopen(z, nasademFileName.c_str(), 0);
  zip_fread(f, contents.get(), st.size);
  zip_fclose(f);
  zip_close(z);

  return contents;
}

// ____________________________________________________________________________
std::unique_ptr<uint8_t[]> NasademFile::getDataInvalid() {
  _length = 2;
  auto contents = std::make_unique<uint8_t[]>(_length);
  contents.get()[0] = 0b00100111;  // 39
  contents.get()[1] = 0b00010000;  // 16
  // Results in 16-bit signed int array [10000].

  return contents;
}

// ____________________________________________________________________________
Coordinate NasademFile::getCellCenter(const Cell& cell) const {
  Coordinate cellCenter((_lon + (cell.getX() * _cellSize)),
                        (_lat + 1 - (cell.getY() * _cellSize)));
  return cellCenter;
}

// ____________________________________________________________________________
Cell NasademFile::getCellFromCoord(const Coordinate& coord) const {
  // Calculate given the row major order.
  const u_int16_t col = ceil((coord.getX() - _lon - _cellCenterOffset) *
                        static_cast<double>(_samples - 1));
  u_int16_t row = ceil((_lat + 1 - coord.getY() - _cellCenterOffset) *
                  static_cast<double>(_samples - 1));
  return Cell(col, row);
}

// ____________________________________________________________________________
int16_t NasademFile::getElevationFromCell(const Cell& cell) const {
  if (cell.getX() > (_samples - 1) || cell.getY() > (_samples - 1)) {
    return INVALID_ELEV;
  }

  // Calculate the position of (row, col) in the bytearray.
  const uint32_t cellIndex = (cell.getY() * _samples + cell.getX()) * 2;
  int16_t elevation = (int16_t)(_data.get()[cellIndex] << 8) +
                      _data.get()[cellIndex + 1];

  if (elevation < MIN_ELEV_EARTH || elevation > MAX_ELEV_EARTH) {
    elevation = INVALID_ELEV;
  }
  return elevation;
}

// ____________________________________________________________________________
int16_t NasademFile::getElevationFromCoord(const Coordinate& coord) const {
  const Cell cell = getCellFromCoord(coord);
  return getElevationFromCell(cell);
}
