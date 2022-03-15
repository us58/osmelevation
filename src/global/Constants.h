// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_GLOBAL_CONSTANTS_H_
#define SRC_GLOBAL_CONSTANTS_H_

#include <cstdint>

namespace global {

// Constants related to NASADEM files.
static const int16_t NASADEM_MIN_LAT = -57;
static const int16_t NASADEM_MAX_LAT = 59;
static const int16_t INVALID_ELEV_NASADEM = -32768;

// General constants related to the earths characteristics.
static const int16_t MIN_ELEV_EARTH = -600;
static const int16_t MAX_ELEV_EARTH = 9000;

///////////////////////////////
// Internal constants
///////////////////////////////

// The default elevation tag added to the osm data.
static const char DEFAULT_ELE_TAG[] = "ele";

// Internally used as invalid elevation / no elevation data available.
static const int16_t INVALID_ELEV = -1000;
static const float INVALID_ELEV_F = static_cast<float>(INVALID_ELEV);

}  // namespace global

#endif  // SRC_GLOBAL_CONSTANTS_H_
