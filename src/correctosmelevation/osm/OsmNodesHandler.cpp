// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <iostream>
#include <string>
#include <osmium/osm/node.hpp>
#include "global/Constants.h"
#include "util/index/NodeIndex.h"
#include "parser/OsmHandler.h"
#include "correctosmelevation/osm/OsmNodesHandler.h"

using global::INVALID_ELEV;
using correctosmelevation::osm::OsmNodesHandler;
using util::index::NodeIndex;
using parser::OsmHandler;

// ____________________________________________________________________________
OsmNodesHandler::OsmNodesHandler(NodeIndex& nodeIndex,
                                const std::string& elevationTag,
                                const std::set<uint64_t>& nodeIds) :
                                _nodeIndex(nodeIndex),
                                _elevationTag(elevationTag),
                                _nodeIds(nodeIds) {}

// ____________________________________________________________________________
void OsmNodesHandler::node(const osmium::Node& node) {
  ++_count;  // For progess bar.
  const osmium::TagList& tags = node.tags();
  if (_nodeIds.contains(node.id())) {
    const char* elevation = tags[_elevationTag.c_str()];
    const int16_t elevationInt = (elevation) ? atoi(elevation) : INVALID_ELEV;
    _nodeIndex.setNode(node.id(), node.location().lon(),
                       node.location().lat(), elevationInt);
  }
}
