// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include <osmium/osm/node.hpp>
#include <osmium/osm/object.hpp>
#include <osmium/memory/buffer.hpp>
#include <osmium/builder/osm_object_builder.hpp>
#include "util/index/ElevationIndexSparse.h"
#include "osmelevation/elevation/GeoElevation.h"
#include "osmelevation/osm/OsmNodesHandler.h"

using osmelevation::elevation::GeoElevation;
using osmelevation::osm::OsmNodesHandler;
using util::index::ElevationIndexSparse;
using GeoTile = std::tuple<int16_t, int16_t, int16_t, int16_t>;

void setNode(osmium::Node& node, const uint64_t id,
             const double lon, const double lat);

// ____________________________________________________________________________
void setNode(osmium::Node& node, const uint64_t id,
             const double lon, const double lat) {
  node.set_id(id);
  node.set_location(osmium::Location{ lon, lat });
}

// ____________________________________________________________________________
TEST(ElevationsInTileTest, node) {
  GeoElevation geoElevation("./");
  ElevationIndexSparse elevationIndex(10, 10);

  const std::vector<GeoTile> tiles = { GeoTile(0, 0, 1, 1),   // tile 1
                                       GeoTile(0, 1, 1, 2),   // tile 2
                                       GeoTile(1, 1, 2, 2),   // tile 3
                                       GeoTile(1, 0, 2, 1) }; // tile 4

  osmium::memory::Buffer nodeBuffer{10240,
                                    osmium::memory::Buffer::auto_grow::yes};
  osmium::builder::NodeBuilder builder{nodeBuffer};
  osmium::Node& node = builder.object();

  for (const auto& tile : tiles) {
    OsmNodesHandler handler(elevationIndex, geoElevation, tile);

    setNode(node, 1, 0, 0);  // tile 1
    handler.node(node);
    setNode(node, 2, 0.5, 0.5);  // tile 1 
    handler.node(node);
    setNode(node, 3, 1, 1);  // tile 1, 2, 3, 4
    handler.node(node);
    setNode(node, 4, 0.5, 1.5);  // tile 2
    handler.node(node);
    setNode(node, 5, 0, 1.5);  // tile 2
    handler.node(node);
    setNode(node, 6, 0.5, 1.9);  // tile 2
    handler.node(node);
    setNode(node, 7, 1, 1.5);  // tile 2, 3
    handler.node(node);
    setNode(node, 8, 1.5, 1.5);  // tile 3
    handler.node(node);
    setNode(node, 9, 1.9, 1.5);  // tile 3
    handler.node(node);
    setNode(node, 10, 1.5, 0);  // tile 4
    handler.node(node);
    setNode(node, 11, 1.9, 0);  // tile 4
    handler.node(node);
    setNode(node, 12, 1.9, 0.5);  // tile 4
    handler.node(node);
    setNode(node, 13, 1.5, 0.5);  // tile 4
    handler.node(node);
    setNode(node, 13, 1, 0.5);  // tile 1, 4
    handler.node(node);
    setNode(node, 14, 10, 10);  // in no tile
    handler.node(node);
  }

  elevationIndex.process();
  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(100, elevationIndex.getElevation(2));
  ASSERT_EQ(100, elevationIndex.getElevation(3));
  ASSERT_EQ(100, elevationIndex.getElevation(4));
  ASSERT_EQ(100, elevationIndex.getElevation(5));
  ASSERT_EQ(100, elevationIndex.getElevation(6));
  ASSERT_EQ(100, elevationIndex.getElevation(7));
  ASSERT_EQ(100, elevationIndex.getElevation(8));
  ASSERT_EQ(100, elevationIndex.getElevation(9));
  ASSERT_EQ(100, elevationIndex.getElevation(10));
  ASSERT_EQ(100, elevationIndex.getElevation(11));
  ASSERT_EQ(100, elevationIndex.getElevation(12));
  ASSERT_EQ(100, elevationIndex.getElevation(13));
  ASSERT_EQ(-1000, elevationIndex.getElevation(14));
}
