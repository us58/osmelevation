// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_CORRECTOSMELEVATION_OSM_OSMNODESHANDLER_H_
#define SRC_CORRECTOSMELEVATION_OSM_OSMNODESHANDLER_H_

#include <set>
#include <string>
#include "parser/OsmHandler.h"
#include "util/index/NodeIndex.h"

namespace correctosmelevation {
namespace osm {

using util::index::NodeIndex;
using parser::OsmHandler;

/*
 * Osm handler to build a node index given a set of
 * needed ids.
 */
class OsmNodesHandler : public OsmHandler {
 public:
  explicit OsmNodesHandler(NodeIndex& nodeIndex,
                           const std::string& elevationTag,
                           const std::set<uint64_t>& nodeIds);

  // Gets called for each node.
  void node(const osmium::Node&) override;

  // Gets called for each way.
  void way(const osmium::Way&) override {};

  // Gets called for each relation.
  void relation(const osmium::Relation&) override {};

 private:
  // The node index where the needed nodes data gets stored.
  NodeIndex& _nodeIndex;

  // The elevation tag to look out for.
  const std::string& _elevationTag;

  // Contains the node ids that are needed.
  const std::set<uint64_t>& _nodeIds;
};

}  // namespace osm
}  // namespace correctosmelevation

#endif  // SRC_CORRECTOSMELEVATION_OSM_OSMNODESHANDLER_H_
