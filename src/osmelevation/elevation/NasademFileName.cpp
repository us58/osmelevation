// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <filesystem>
#include <string>
#include "util/geo/Point.h"
#include "osmelevation/elevation/NasademFileName.h"

using CoordInt = util::geo::Point<int16_t>;

// ____________________________________________________________________________
std::string osmelevation::elevation::coordPadZeros(const u_int16_t& number,
                                                   const u_int8_t& pad_to) {
  std::string result = std::to_string(number);
  if (pad_to > result.length()) {
    result.insert(0, pad_to - result.length(), '0');
  }
  return result;
}

// ____________________________________________________________________________
std::string osmelevation::elevation::convertToNasademNaming(
    const CoordInt& coord) {
  std::string ew = (coord.getX() >= 0) ?
    std::string(1, 'e') : std::string(1, 'w');
  std::string ns = (coord.getY() >= 0) ?
    std::string(1, 'n') : std::string(1, 's');

  // Construct the correct file format given the coordinate.
  return ns.append(coordPadZeros(abs(coord.getY()),
                   2)).append(ew).append(coordPadZeros(abs(coord.getX()), 3));
}

// ____________________________________________________________________________
std::string osmelevation::elevation::getNasademFilePath(
    std::string nasademDir, const std::string& fileName) {
  const std::string prefix = "NASADEM_HGT_";
  const std::string extension = ".zip";
  std::string nasademFp;
  nasademFp = nasademDir.append(prefix).append(fileName).append(extension);

  // Check if the requested file exists.
  if (!std::filesystem::exists(nasademFp)) {
    return std::string("invalid");
  }
  return nasademFp;
}
