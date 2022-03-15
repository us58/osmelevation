// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <cstdint>
#include <vector>
#include <tuple>
#include "util/osm/OsmStats.h"
#include "global/Constants.h"
#include "osmelevation/osm/GeoBoundaries.h"

using osmelevation::osm::GeoBoundaries;
using util::osm::OsmStats;
using global::NASADEM_MIN_LAT;
using global::NASADEM_MAX_LAT;
using GeoBoundary = std::tuple<int16_t, int16_t, int16_t, int16_t>;

// ____________________________________________________________________________
GeoBoundaries::GeoBoundaries(
    const uint8_t size, const OsmStats& osmStats) :
    _size(size), _osmStats(osmStats) {}

// ____________________________________________________________________________
std::vector<GeoBoundary> GeoBoundaries::buildBoundaries() const {
  auto boundaries = std::vector<GeoBoundary>();

  const int16_t minLatNasadem = (_osmStats.minLat < NASADEM_MIN_LAT)
                                ? NASADEM_MIN_LAT : _osmStats.minLat;
  const int16_t maxLatNasadem = (_osmStats.maxLat > NASADEM_MAX_LAT)
                                ? NASADEM_MAX_LAT : _osmStats.maxLat;

  for (int16_t lon = _osmStats.minLon; lon < _osmStats.maxLon;
       lon = lon + _size) {
    for (int16_t lat = minLatNasadem; lat < maxLatNasadem;
         lat = lat + _size) {
      int16_t minLon = lon;
      int16_t minLat = lat;
      int16_t maxLon = lon + _size;
      int16_t maxLat = lat + _size;
      maxLat = (maxLat > maxLatNasadem) ? maxLatNasadem : maxLat;
      boundaries.emplace_back(minLon, minLat, maxLon, maxLat);
    }
  }
  return boundaries;
}
