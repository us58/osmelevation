// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_OSM_GETOSMSTATS_H_
#define SRC_UTIL_OSM_GETOSMSTATS_H_

#include <cstdint>
#include "parser/OsmHandler.h"
#include "util/osm/OsmStats.h"

namespace util {
namespace osm {

using util::osm::OsmStats;
using parser::OsmHandler;

class GetOsmStats : public OsmHandler {
 public:
  GetOsmStats();

  OsmStats getOsmStats();

  // Gets called for each node.
  void node(const osmium::Node&) override;

  // Gets called for each way.
  void way(const osmium::Way&) override;

  // Gets called for each relation.
  void relation(const osmium::Relation&) override;

 private:
  // The minimum node ID present in the osm file.
  int64_t _min;

  // The maximum node ID present in the osm file.
  int64_t _max;

  // The number of nodes/ways/relations counted.
  uint64_t _nodeCount;
  uint64_t _wayCount;
  uint64_t _relationCount;

  double _minLon;
  double _minLat;
  double _maxLon;
  double _maxLat;
};

}  // namespace osm
}  // namespace util

#endif  // SRC_UTIL_OSM_GETOSMSTATS_H_
