// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_OSMELEVATION_ELEVATION_NASADEMFILENAME_H_
#define SRC_OSMELEVATION_ELEVATION_NASADEMFILENAME_H_

#include <string>
#include "util/geo/Point.h"

using CoordInt = util::geo::Point<int16_t>;

namespace osmelevation {
namespace elevation {

// Add num_zeros zeros in front of a number and return it as string.
std::string coordPadZeros(const u_int16_t& number, const u_int8_t& num_zeros);

// Given a coordinate, convert it to the NASADEM naming scheme.
std::string convertToNasademNaming(const CoordInt& coord);

// Given the complete NASADEM file name, get the file path to the file.
// If the file doesn't exist, return invalid.
std::string getNasademFilePath(std::string nasademDir,
                               const std::string& fileName);

}  // namespace elevation
}  // namespace osmelevation

#endif  // SRC_OSMELEVATION_ELEVATION_NASADEMFILENAME_H_
