// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_OSMWAYSHANDLER_H_
#define SRC_CORRECTOSMELEVATION_OSM_OSMWAYSHANDLER_H_

#include <vector>
#include <set>
#include <cstdint>
#include "parser/OsmHandler.h"

namespace correctosmelevation {
namespace osm {

using parser::OsmHandler;
using RoutePaths = std::vector<std::vector<uint64_t>>;

/*
 * Collect all route paths from ways in a specified routes range.
 */
class OsmWaysHandler : public OsmHandler {
 public:
  OsmWaysHandler(std::vector<RoutePaths>& routePaths,
                 const std::set<uint64_t>& correctedWaysIds,
                 const uint64_t& rangeStart,
                 const uint64_t& rangeEnd);

  // Gets called for each node.
  void node(const osmium::Node&) override {};

  // Gets called for each way.
  void way(const osmium::Way&) override;

  // Gets called for each relation.
  void relation(const osmium::Relation&) override {};

 private:
  // Store the found route paths.
  std::vector<RoutePaths>& _routePaths;

  // Do not collect route paths from ways that were already
  // corrected in route relations.
  std::set<uint64_t> _correctedWaysIds;

  // The bounds of the routes range.
  const uint64_t _rangeStart;
  const uint64_t _rangeEnd;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_OSMWAYSHANDLER_H_
