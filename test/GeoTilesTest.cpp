// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include <tuple>
#include "util/osm/OsmStats.h"
#include "osmelevation/osm/GeoBoundaries.h"

using osmelevation::osm::GeoBoundaries;
using util::osm::OsmStats;

// ____________________________________________________________________________
TEST(GeoTilesTest, constructSectionsOneTile) {
  OsmStats osmStats;
  osmStats.minLon = -10;
  osmStats.minLat = -10;
  osmStats.maxLon = 10;
  osmStats.maxLat = 10;

  GeoBoundaries geoTiles(20, osmStats);

  const auto tiles = geoTiles.buildBoundaries();

  ASSERT_EQ((size_t)1, tiles.size());
  ASSERT_EQ(-10, std::get<0>(tiles[0]));
  ASSERT_EQ(-10, std::get<1>(tiles[0]));
  ASSERT_EQ(10, std::get<2>(tiles[0]));
  ASSERT_EQ(10, std::get<3>(tiles[0]));
}

// ____________________________________________________________________________
TEST(GeoTilesTest, constructSectionsMultipleTiles) {
  OsmStats osmStats;
  osmStats.minLon = -29;
  osmStats.minLat = -21;
  osmStats.maxLon = 20;
  osmStats.maxLat = 10;

  GeoBoundaries geoTiles(20, osmStats);
  const auto tiles = geoTiles.buildBoundaries();

  ASSERT_EQ((size_t)6, tiles.size());
  ASSERT_EQ(-29, std::get<0>(tiles[0]));
  ASSERT_EQ(-21, std::get<1>(tiles[0]));
  ASSERT_EQ(-9, std::get<2>(tiles[0]));
  ASSERT_EQ(-1, std::get<3>(tiles[0]));

  ASSERT_EQ(-29, std::get<0>(tiles[1]));
  ASSERT_EQ(-1, std::get<1>(tiles[1]));
  ASSERT_EQ(-9, std::get<2>(tiles[1]));
  ASSERT_EQ(10, std::get<3>(tiles[1]));

  ASSERT_EQ(-9, std::get<0>(tiles[2]));
  ASSERT_EQ(-21, std::get<1>(tiles[2]));
  ASSERT_EQ(11, std::get<2>(tiles[2]));
  ASSERT_EQ(-1, std::get<3>(tiles[2]));

  ASSERT_EQ(-9, std::get<0>(tiles[3]));
  ASSERT_EQ(-1, std::get<1>(tiles[3]));
  ASSERT_EQ(11, std::get<2>(tiles[3]));
  ASSERT_EQ(10, std::get<3>(tiles[3]));

  ASSERT_EQ(11, std::get<0>(tiles[4]));
  ASSERT_EQ(-21, std::get<1>(tiles[4]));
  ASSERT_EQ(31, std::get<2>(tiles[4]));
  ASSERT_EQ(-1, std::get<3>(tiles[4]));

  ASSERT_EQ(11, std::get<0>(tiles[5]));
  ASSERT_EQ(-1, std::get<1>(tiles[5]));
  ASSERT_EQ(31, std::get<2>(tiles[5]));
  ASSERT_EQ(10, std::get<3>(tiles[5]));
}

// ____________________________________________________________________________
TEST(GeoTilesTest, constructSectionsMultipleOverNasademLat) {
  OsmStats osmStats;
  osmStats.minLon = -9;
  osmStats.minLat = -81;
  osmStats.maxLon = 5;
  osmStats.maxLat = 76;

  GeoBoundaries geoTiles(20, osmStats);
  const auto tiles = geoTiles.buildBoundaries();

  ASSERT_EQ((size_t)6, tiles.size());
  ASSERT_EQ(-9, std::get<0>(tiles[0]));
  ASSERT_EQ(-57, std::get<1>(tiles[0]));
  ASSERT_EQ(11, std::get<2>(tiles[0]));
  ASSERT_EQ(-37, std::get<3>(tiles[0]));

  ASSERT_EQ(-9, std::get<0>(tiles[1]));
  ASSERT_EQ(-37, std::get<1>(tiles[1]));
  ASSERT_EQ(11, std::get<2>(tiles[1]));
  ASSERT_EQ(-17, std::get<3>(tiles[1]));

  ASSERT_EQ(-9, std::get<0>(tiles[2]));
  ASSERT_EQ(-17, std::get<1>(tiles[2]));
  ASSERT_EQ(11, std::get<2>(tiles[2]));
  ASSERT_EQ(3, std::get<3>(tiles[2]));

  ASSERT_EQ(-9, std::get<0>(tiles[3]));
  ASSERT_EQ(3, std::get<1>(tiles[3]));
  ASSERT_EQ(11, std::get<2>(tiles[3]));
  ASSERT_EQ(23, std::get<3>(tiles[3]));

  ASSERT_EQ(-9, std::get<0>(tiles[4]));
  ASSERT_EQ(23, std::get<1>(tiles[4]));
  ASSERT_EQ(11, std::get<2>(tiles[4]));
  ASSERT_EQ(43, std::get<3>(tiles[4]));

  ASSERT_EQ(-9, std::get<0>(tiles[5]));
  ASSERT_EQ(43, std::get<1>(tiles[5]));
  ASSERT_EQ(11, std::get<2>(tiles[5]));
  ASSERT_EQ(59, std::get<3>(tiles[5]));
}

// ____________________________________________________________________________
TEST(GeoTilesTest, constructSectionsNoTile) {
  OsmStats osmStats;
  osmStats.minLon = 0;
  osmStats.minLat = 0;
  osmStats.maxLon = 0;
  osmStats.maxLat = 0;

  GeoBoundaries geoTiles(20, osmStats);
  const auto tiles = geoTiles.buildBoundaries();

  ASSERT_EQ((size_t)0, tiles.size());
}
