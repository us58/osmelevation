// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_OSMELEVATION_OSM_OSMNODESHANDLER_H_
#define SRC_OSMELEVATION_OSM_OSMNODESHANDLER_H_

#include <tuple>
#include <cstdint>
#include "parser/OsmHandler.h"
#include "osmelevation/elevation/GeoElevation.h"
#include "util/index/ElevationIndex.h"

namespace osmelevation {
namespace osm {

using osmelevation::elevation::GeoElevation;
using util::index::ElevationIndex;
using parser::OsmHandler;
using Partition = std::tuple<int16_t, int16_t, int16_t, int16_t>;

/*
 * OSM handler that derives from osmium::handler::Handler.
 * The implemented node function checks if a node (location)
 * is inside the provided geographical partition.
 * If yes, further processing will occur.
 * Ultimately, multiple instances of this handler are used
 * to work off all nodes of an OSM file.
 * This way, the order of which all nodes are being processed
 * provides a geographical clustering of the nodes.
 */
class OsmNodesHandler : public OsmHandler {
 public:
  OsmNodesHandler(ElevationIndex& elevationIndex,
                  GeoElevation& geoElevation,
                  const Partition& geoPartition);

  // Gets called for each node. Check if the node is inside
  // the geo partition and further process if so.
  void node(const osmium::Node&) override;

  // Gets called for each way (not implemented).
  void way(const osmium::Way&) override {};

  // Gets called for each relation (not implemented).
  void relation(const osmium::Relation&) override {};

 private:
  // Elevation index to store the results.
  ElevationIndex& _elevationIndex;

  // Interface to get the elevation of a location.
  GeoElevation& _geoElevation;

  // The geographic partition provided by its
  // bottom-left and top-right coordinate
  // (minlon, minlat, maxlon, maxlat).
  const Partition& _geoPartition;
};

}  // namespace osm
}  // namespace osmelevation

#endif  // SRC_OSMELEVATION_OSM_OSMNODESHANDLER_H_
