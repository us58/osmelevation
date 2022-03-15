// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <iostream>
#include <tuple>
#include "util/index/ElevationIndex.h"
#include "util/osm/OsmStats.h"
#include "parser/NodeParser.h"
#include "osmelevation/elevation/GeoElevation.h"
#include "osmelevation/osm/OsmNodesHandler.h"
#include "osmelevation/osm/GeoPartition.h"

using parser::NodeParser;
using util::index::ElevationIndex;
using util::osm::OsmStats;
using osmelevation::osm::OsmNodesHandler;
using osmelevation::elevation::GeoElevation;
using osmelevation::osm::GeoPartition;
using GeoBoundary = std::tuple<int16_t, int16_t, int16_t, int16_t>;

// _____________________________________________________________________________
GeoPartition::GeoPartition(ElevationIndex& elevationIndex,
                           const OsmStats& osmStats,
                           const std::string& inFile,
                           const std::string& nasademDir,
                           const GeoBoundary& boundary) :
                           _elevationIndex(elevationIndex),
                           _osmStats(osmStats),
                           _inFile(inFile),
                           _nasademDir(nasademDir),
                           _boundary(boundary) {}

// _____________________________________________________________________________
void GeoPartition::elevationsInPartition() {
  GeoElevation geoElevation(_nasademDir);
  std::cout << "\nWorking on geographic partition with minlon: ";
  std::cout << std::get<0>(_boundary) << ", ";
  std::cout << "minlat: " << std::get<1>(_boundary) << ", ";
  std::cout << "maxlon: " << std::get<2>(_boundary) << ", ";
  std::cout << "maxlat: " << std::get<3>(_boundary) << std::endl;

  OsmNodesHandler handler(_elevationIndex, geoElevation, _boundary);
  NodeParser parser(_inFile, &handler, _osmStats);
  parser.parse();
  geoElevation.clear();
}
