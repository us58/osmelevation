// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <math.h>
#include <osmium/osm.hpp>
#include "util/osm/OsmStats.h"
#include "util/osm/GetOsmStats.h"

using util::osm::GetOsmStats;
using util::osm::OsmStats;

// ____________________________________________________________________________
GetOsmStats::GetOsmStats() {
  _min = 100000000000;
  _max = 0;

  _nodeCount = 0;
  _wayCount = 0;
  _relationCount = 0;

  _minLon = 180.0;
  _minLat = 90.0;
  _maxLon = -180.0;
  _maxLat = -90.0;
}

// ____________________________________________________________________________
OsmStats GetOsmStats::getOsmStats() {
  OsmStats osmStats;

  // Round up or down accordingly and also adjust to the NASADEM bounds.
  osmStats.minLon = floor(_minLon);
  osmStats.minLat = (_minLat <= -57) ? -57 : floor(_minLat);
  osmStats.maxLon = ceil(_maxLon);
  osmStats.maxLat = (_maxLat >= 61) ? 61 : ceil(_maxLat);
  osmStats.nodeCount = _nodeCount;
  osmStats.wayCount = _wayCount;
  osmStats.relationCount = _relationCount;
  osmStats.min = _min;
  osmStats.max = _max;

  return osmStats;
}

// ____________________________________________________________________________
void GetOsmStats::node(const osmium::Node& node) {
  ++_nodeCount;
  if (node.id() < _min) { _min = node.id(); }
  if (node.id() > _max) { _max = node.id(); }

  double lon = node.location().lon();
  double lat = node.location().lat();

  // Get the bounds of the osm file.
  if (lon < _minLon) { _minLon = lon; }
  if (lat < _minLat) { _minLat = lat; }
  if (lon > _maxLon) { _maxLon = lon; }
  if (lat > _maxLat) { _maxLat = lat; }
}

// ____________________________________________________________________________
void GetOsmStats::way(__attribute__((unused)) const osmium::Way& way) {
  ++_wayCount;
}

// ____________________________________________________________________________
void GetOsmStats::relation(
    __attribute__((unused)) const osmium::Relation& relation) {
  ++_relationCount;
}
