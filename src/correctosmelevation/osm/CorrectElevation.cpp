// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <cstdint>
#include <memory>
#include <ctime>
#include "writer/OsmAddElevationWriter.h"
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "util/osm/OsmStats.h"
#include "util/osm/GetOsmStats.h"
#include "parser/NodeWayRelationParser.h"
#include "correctosmelevation/correct/SmoothRoute.h"
#include "correctosmelevation/correct/CorrectRiver.h"
#include "correctosmelevation/correct/CorrectTunnelOrBridge.h"
#include "correctosmelevation/osm/RoutesFromRelations.h"
#include "correctosmelevation/osm/RoutesFromWays.h"
#include "correctosmelevation/osm/CorrectElevation.h"

using writer::OsmAddElevationWriter;
using correctosmelevation::osm::CorrectElevation;
using correctosmelevation::osm::RoutesFromRelations;
using correctosmelevation::osm::RoutesFromWays;
using correctosmelevation::correct::SmoothRoute;
using correctosmelevation::correct::CorrectRiver;
using correctosmelevation::correct::CorrectTunnelOrBridge;
using util::index::NodeIndex;
using parser::NodeWayRelationParser;
using util::osm::OsmStats;
using util::osm::GetOsmStats;
using util::index::AverageElevationIndexSparse;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// _____________________________________________________________________________
CorrectElevation::CorrectElevation(const std::string inFile,
                                   const std::string outFile,
                                   const std::string elevationTag,
                                   const uint64_t waysPerRange,
                                   const uint64_t relationsPerRange) :
                                   _inFile(inFile),
                                   _outFile(outFile),
                                   _elevationTag(elevationTag),
                                   _waysPerRange(waysPerRange),
                                   _relationsPerRange(relationsPerRange) {}

// _____________________________________________________________________________
void CorrectElevation::initialize() {
  GetOsmStats handler;
  NodeWayRelationParser statsParser(_inFile, &handler);

  time_t start, end;
  start = time(&start);
  std::cout << "Collecting statistics about the input file ";
  std::cout << _inFile << std::endl;

  statsParser.parse();

  end = time(&end);
  double timeDiff = difftime(end, start);
  std::cout << "Done, took " << timeDiff;
  std::cout << " seconds." << "\n" << std::endl;

  _osmStats = handler.getOsmStats();
  _elevationIndex =
    std::make_unique<AverageElevationIndexSparse>(_osmStats.nodeCount / 2,
                                                  _osmStats.max);
  _elevationIndex->process();
}

// _____________________________________________________________________________
int16_t CorrectElevation::getElevation(const uint64_t id) const {
  return _elevationIndex->getElevation(id);
}

// _____________________________________________________________________________
void CorrectElevation::correctRoutes() const {
  const uint64_t absoluteMaxWays = _osmStats.wayCount;
  const uint64_t absoluteMaxRelations = _osmStats.relationCount;

  std::cout << "Correct routes found in osm relations." << "\n" << std::endl;
  const auto correctedWayIds =
      correctRouteRelationsInRanges(absoluteMaxRelations, _relationsPerRange);

  std::cout << "Correct all remaining routes found in osm ways." << std::endl;
  correctRouteWaysInRanges(correctedWayIds, absoluteMaxWays, _waysPerRange);
}

// _____________________________________________________________________________
std::set<uint64_t> CorrectElevation::correctRouteRelationsInRanges(
    const uint64_t absoluteMax, const uint64_t maxPerLoop) const {
  std::set<uint64_t> correctedWayIds;
  uint64_t currentStart = 0;
  uint64_t currentEnd = maxPerLoop;

  while (currentStart < absoluteMax) {
    std::vector<RoutePaths> routePaths;
    std::vector<RoutePaths> rivers;
    std::vector<RoutePaths> tunnelsAndBridges;
    auto routesFromRelations =
        RoutesFromRelations(_inFile, _osmStats, _elevationTag,
                            currentStart, currentEnd);
    routesFromRelations.getRoutesAndIds(correctedWayIds, routePaths, rivers,
                                        tunnelsAndBridges);

    // If no more route paths or rivers were found, we are done.
    if (routePaths.empty() && rivers.empty()) {
      break;
    }
    // Make a set containing all node ids that are needed.
    const std::set<uint64_t> nodeIds =
        routesFromRelations.requiredNodes(routePaths, rivers);

    // Build a node index with needed ids.
    NodeIndex nodeIndex = routesFromRelations.buildNodeIndex(nodeIds);

    // Correct the different types of routes.
    correctRivers(nodeIndex, rivers);
    correctTunnelsAndBridges(nodeIndex, tunnelsAndBridges);
    smoothRoutePaths(nodeIndex, routePaths);

    nodeIndex.clear();
    _elevationIndex->process();

    currentStart += maxPerLoop;
    currentEnd += maxPerLoop;
  }
  return correctedWayIds;
}

// _____________________________________________________________________________
void CorrectElevation::correctRouteWaysInRanges(
    const std::set<uint64_t>& correctedWayIds,
    const uint64_t absoluteMax, const uint64_t maxPerLoop) const {
  uint64_t currentStart = 0;
  uint64_t currentEnd = maxPerLoop;
  const std::vector<RoutePaths> routePaths;

  while (currentStart < absoluteMax) {
    auto routesFromWays =
        RoutesFromWays(_inFile, _osmStats, _elevationTag,
                       currentStart, currentEnd);
    const auto routes = routesFromWays.getRoutesAndExcludeIds(correctedWayIds);

    // If there are no more routes to collect, we are done.
    if (routes.empty()) {
      break;
    }
    // Make a set containing all node ids that are needed.
    const std::set<uint64_t> nodeIds =
        routesFromWays.requiredNodes(routes, routePaths);

    // Build a node index with needed ids.
    NodeIndex nodeIndex = routesFromWays.buildNodeIndex(nodeIds);

    // Only smoothing for route ways.
    smoothRoutePaths(nodeIndex, routes);

    nodeIndex.clear();
    _elevationIndex->process();

    currentStart += maxPerLoop;
    currentEnd += maxPerLoop;
  }
}

// _____________________________________________________________________________
void CorrectElevation::correctRivers(NodeIndex& nodeIndex,
                                     std::vector<RoutePaths>& rivers) const {
  for (const auto& river : rivers) {
    std::vector<uint64_t> nodeCounts;
    for (const auto& subroute : river) {
      nodeCounts.emplace_back(subroute.size());
    }
    CorrectRiver correctRiver(river, nodeCounts, nodeIndex, *_elevationIndex);
    correctRiver.buildElevations();
    correctRiver.buildRiverNodes();
    correctRiver.correctDownstreamElevation();
    correctRiver.updateElevationIndex();
  }
}

// _____________________________________________________________________________
void CorrectElevation::correctTunnelsAndBridges(
    NodeIndex& nodeIndex,
    std::vector<RoutePaths>& tunnelsAndBridges) const {
  for (const auto& tunnelOrBridge : tunnelsAndBridges) {
    std::vector<uint64_t> nodeCounts;
    for (const auto& subroute : tunnelOrBridge) {
      nodeCounts.emplace_back(subroute.size());
    }
    CorrectTunnelOrBridge correctTunnelOrBridge(tunnelOrBridge, nodeCounts,
                                                nodeIndex, *_elevationIndex);
    correctTunnelOrBridge.buildCoordsAndElevations();
    correctTunnelOrBridge.createElevationPlane();
    correctTunnelOrBridge.estimateUnknownSection();
  }
}

// _____________________________________________________________________________
void CorrectElevation::smoothRoutePaths(NodeIndex& nodeIndex,
    const std::vector<RoutePaths>& routePaths) const {
  // Smooth all route paths in form of all found route ways.
  for (const auto& routePath : routePaths) {
    if (routePath.size() == 0) { continue; }
    const auto nodeCounts = std::vector<size_t> { routePath[0].size() };
    SmoothRoute smooth(routePath, nodeCounts, nodeIndex, *_elevationIndex);
    smooth.buildCoordsAndElevations();
    smooth.buildDistances();
    smooth.smoothMovingAverage(60);
  }
}

// ____________________________________________________________________________
void CorrectElevation::writeOutputOSM() const {
  OsmAddElevationWriter writer(_inFile, _outFile,
                               _elevationIndex.get(),
                               _elevationTag, true);
  writer.write();
}
