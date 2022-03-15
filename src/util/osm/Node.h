// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_OSM_NODE_H_
#define SRC_UTIL_OSM_NODE_H_

#include <cstdint>

namespace util {
namespace osm {

/*
 * Store the node's location, the id and the elevation
 * at the node's location.
 */
struct Node {
 public:
  Node(uint64_t id, double lon, double lat, int16_t elevation) :
       id(id), lon(lon), lat(lat), elevation(elevation) {}

  bool operator==(const Node& n) {
    return n.id == id;
  }

  // The id of the node.
  uint64_t id;

  // The longitude coordinate of the node.
  double lon;

  // The latitude coordinate of the node.
  double lat;

  // The elevation at the node's location in meters.
  int16_t elevation;
};

}  // namespace osm
}  // namespace util

#endif  // SRC_UTIL_OSM_NODE_H_
