// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <set>
#include <vector>
#include <cstdint>
#include <osmium/io/any_input.hpp>
#include "parser/WayParser.h"
#include "correctosmelevation/osm/OsmWaysHandler.h"
#include "correctosmelevation/osm/RoutesFromWays.h"

using parser::WayParser;
using correctosmelevation::osm::OsmWaysHandler;
using correctosmelevation::osm::RoutesFromWays;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// _____________________________________________________________________________
std::vector<RoutePaths> RoutesFromWays::getRoutesAndExcludeIds(
    const std::set<uint64_t>& correctedWayIds) const {
  std::vector<RoutePaths> routes;
  OsmWaysHandler osmWayshandler(routes, correctedWayIds,
                                _rangeStart, _rangeEnd);
  WayParser wParser(_osmFile, &osmWayshandler, _osmStats);
  wParser.parseNoProgressBar();

  return routes;
}
