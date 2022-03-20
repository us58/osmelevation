// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include <cstdint>
#include <osmium/osm/way.hpp>
#include "correctosmelevation/correct/SmoothRoute.h"
#include "correctosmelevation/osm/OsmWaysHandler.h"

using correctosmelevation::osm::OsmWaysHandler;
using correctosmelevation::correct::SmoothRoute;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
OsmWaysHandler::OsmWaysHandler(
    std::vector<RoutePaths>& routePaths,
    const std::set<uint64_t>& correctedWaysIds,
    const uint64_t& rangeStart, const uint64_t& rangeEnd) :
    _routePaths(routePaths), _correctedWaysIds(correctedWaysIds),
    _rangeStart(rangeStart), _rangeEnd(rangeEnd) {}

// ____________________________________________________________________________
void OsmWaysHandler::way(const osmium::Way& way) {
  const bool highway = way.tags().has_key("highway");
  if (highway) {
    if (!_correctedWaysIds.contains(way.id())) {
      if (_count >= _rangeStart && _count < _rangeEnd) {
        const size_t nodeCount = way.nodes().size();
        RoutePaths routePath;
        routePath.emplace_back(std::vector<uint64_t>());
        routePath[0].reserve(nodeCount);
        for (size_t i = 0; i < nodeCount; ++i) {
          routePath[0].emplace_back(way.nodes()[i].ref());
        }
        _routePaths.emplace_back(routePath);
      }
      ++_count;
    }
  }
}
