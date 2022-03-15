// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <set>
#include <vector>
#include <cstdint>
#include <osmium/io/any_input.hpp>
#include "correctosmelevation/osm/OsmRelationsManager.h"
#include "correctosmelevation/osm/RoutesFromRelations.h"

using correctosmelevation::osm::OsmRelationsManager;
using correctosmelevation::osm::RoutesFromRelations;
using RoutePaths = std::vector<std::vector<uint64_t>>;

// _____________________________________________________________________________
void RoutesFromRelations::getRoutesAndIds(
    std::set<uint64_t>& correctedWayIds,
    std::vector<RoutePaths>& routePaths,
    std::vector<RoutePaths>& rivers,
    std::vector<RoutePaths>& tunnelsAndBridges) const {

  osmium::io::File inputFile{_osmFile};

  OsmRelationsManager osmRelationsManager(correctedWayIds, routePaths, rivers,
                                          tunnelsAndBridges, _rangeStart,
                                          _rangeEnd);

  osmium::relations::read_relations(inputFile, osmRelationsManager);

  osmium::io::Reader reader{inputFile};
  osmium::apply(reader, osmRelationsManager.handler());
}
