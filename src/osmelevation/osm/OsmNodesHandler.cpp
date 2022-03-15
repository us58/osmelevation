// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <tuple>
#include <cstdint>
#include <osmium/osm/node.hpp>
#include "util/index/ElevationIndex.h"
#include "osmelevation/elevation/GeoElevation.h"
#include "osmelevation/osm/OsmNodesHandler.h"

using osmelevation::elevation::GeoElevation;
using util::index::ElevationIndex;
using osmelevation::osm::OsmNodesHandler;
using Partition = std::tuple<int16_t, int16_t, int16_t, int16_t>;

// ____________________________________________________________________________
OsmNodesHandler::OsmNodesHandler(ElevationIndex& elevationIndex,
                                 GeoElevation& geoElevation,
                                 const Partition& geoPartition) :
                                 _elevationIndex(elevationIndex),
                                 _geoElevation(geoElevation),
                                 _geoPartition(geoPartition) {}

// ____________________________________________________________________________
void OsmNodesHandler::node(const osmium::Node& node) {
  ++_count;
  const double lon = node.location().lon();
  const double lat = node.location().lat();

  // Check if the node's location is inside the geographic partition.
  if (lon >= std::get<0>(_geoPartition) &&
      lon <= std::get<2>(_geoPartition) &&
      lat >= std::get<1>(_geoPartition) &&
      lat <= std::get<3>(_geoPartition)) {
    // Get the elevation at the node's location.
    const int16_t elevation =
      _geoElevation.getInterpolatedElevation(Coordinate(lon, lat));

    // Store the node's id and elevation in the elevation index.
    _elevationIndex.setElevation(node.id(), elevation);
  }
}
