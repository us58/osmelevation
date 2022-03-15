// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <cstdint>
#include <gtest/gtest.h>
#include "parser/NodeWayRelationParser.h"
#include "util/geo/Geo.h"
#include "util/geo/Point.h"
#include "util/console/Console.h"
#include "util/geo/Point.h"
#include "util/geometry/Vector3d.h"
#include "util/osm/OsmStats.h"
#include "util/osm/GetOsmStats.h"

using parser::NodeWayRelationParser;
using Coordinate = util::geo::Point<double>;
using util::geo::haversineApprox;
using util::geo::haversine;
using util::console::parseCommandLineArgumentsAdd;
using util::console::parseCommandLineArgumentsCorrect;
using util::geo::Point;
using util::geometry::Vector3d;
using util::osm::OsmStats;
using util::osm::GetOsmStats;

// ____________________________________________________________________________
TEST(UTILTESTS, haversine) {
  ASSERT_DOUBLE_EQ(111194.92664455874, haversine(Coordinate(0, 0), Coordinate(1, 0)));
  ASSERT_DOUBLE_EQ(111194.92664455874, haversine(Coordinate(0, 0), Coordinate(0, 1)));
  ASSERT_DOUBLE_EQ(71474.188743478924, haversine(Coordinate(0, 50), Coordinate(1, 50)));
  ASSERT_DOUBLE_EQ(111194.92664455889, haversine(Coordinate(0, 50), Coordinate(0, 51)));

  ASSERT_DOUBLE_EQ(6.3062735839906674, haversine(Coordinate(7.75275, 47.92653), Coordinate(7.75269, 47.92649)));
  ASSERT_DOUBLE_EQ(55.482028283396325, haversine(Coordinate(7.75275, 47.92653), Coordinate(7.753, 47.927)));
  ASSERT_DOUBLE_EQ(32254.246746278895, haversine(Coordinate(7.5, 47.8), Coordinate(7.9, 47.91)));
}

// ____________________________________________________________________________
TEST(UTILTESTS, haversineApprox) {
  ASSERT_DOUBLE_EQ(6.3062735839910493, haversineApprox(Coordinate(7.75275, 47.92653), Coordinate(7.75269, 47.92649)));
  ASSERT_DOUBLE_EQ(55.482028283481135, haversineApprox(Coordinate(7.75275, 47.92653), Coordinate(7.753, 47.927)));
}

// ____________________________________________________________________________
TEST(UTILTESTS, parseCommandLineArgumentsAddNoArguments) {
  int argc = 1;
  char* argv[1] = {
    const_cast<char*>("")
  };

  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_DEATH(parseCommandLineArgumentsAdd(argc, argv), "Usage: .*");
}

// ____________________________________________________________________________
TEST(UTILTESTS, parseCommandLineArgumentsAdd) {
  int argc = 4;
  char* argv[4] = {
    const_cast<char*>(""),
    const_cast<char*>("nasadem"),
    const_cast<char*>("input"),
    const_cast<char*>("output")
  };
  const auto args = parseCommandLineArgumentsAdd(argc, argv);
  ASSERT_STREQ("nasadem", args.nasademDir.c_str());
  ASSERT_STREQ("input", args.inputFile.c_str());
  ASSERT_STREQ("output", args.outputFile.c_str());
  ASSERT_STREQ("ele", args.elevationTag.c_str());
}

// ____________________________________________________________________________
TEST(UTILTESTS, parseCommandLineArgumentsAddSetEleTag) {
  int argc = 6;
  char* argv[6] = {
    const_cast<char*>(""),
    const_cast<char*>("--tag"),
    const_cast<char*>("elevation"),
    const_cast<char*>("nasadem"),
    const_cast<char*>("input"),
    const_cast<char*>("output")
  };
  const auto args = parseCommandLineArgumentsAdd(argc, argv);
  ASSERT_STREQ("nasadem", args.nasademDir.c_str());
  ASSERT_STREQ("input", args.inputFile.c_str());
  ASSERT_STREQ("output", args.outputFile.c_str());
  ASSERT_STREQ("elevation", args.elevationTag.c_str());

  int argc1 = 6;
  char* argv1[6] = {
    const_cast<char*>(""),
    const_cast<char*>("-t"),
    const_cast<char*>("elev"),
    const_cast<char*>("nasadem"),
    const_cast<char*>("input"),
    const_cast<char*>("output")
  };
  const auto args1 = parseCommandLineArgumentsAdd(argc1, argv1);
  ASSERT_STREQ("nasadem", args1.nasademDir.c_str());
  ASSERT_STREQ("input", args1.inputFile.c_str());
  ASSERT_STREQ("output", args1.outputFile.c_str());
  ASSERT_STREQ("elev", args1.elevationTag.c_str());
}

// ____________________________________________________________________________
TEST(UTILTESTS, parseCommandLineArgumentsCorrectNoArguments) {
  int argc = 1;
  char* argv[1] = {
    const_cast<char*>("")
  };

  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_DEATH(parseCommandLineArgumentsCorrect(argc, argv), "Usage: .*");
}

// ____________________________________________________________________________
TEST(UTILTESTS, parseCommandLineArgumentsCorrect) {
  int argc = 3;
  char* argv[3] = {
    const_cast<char*>(""),
    const_cast<char*>("input"),
    const_cast<char*>("output")
  };
  const auto args = parseCommandLineArgumentsCorrect(argc, argv);
  ASSERT_STREQ("input", args.inputFile.c_str());
  ASSERT_STREQ("output", args.outputFile.c_str());
  ASSERT_STREQ("ele", args.elevationTag.c_str());
}

// ____________________________________________________________________________
TEST(UTILTESTS, parseCommandLineArgumentsCorrectSetEleTag) {
  int argc = 5;
  char* argv[5] = {
    const_cast<char*>(""),
    const_cast<char*>("--tag"),
    const_cast<char*>("elevation"),
    const_cast<char*>("input"),
    const_cast<char*>("output")
  };
  const auto args = parseCommandLineArgumentsCorrect(argc, argv);
  ASSERT_STREQ("input", args.inputFile.c_str());
  ASSERT_STREQ("output", args.outputFile.c_str());
  ASSERT_STREQ("elevation", args.elevationTag.c_str());

  int argc1 = 5;
  char* argv1[5] = {
    const_cast<char*>(""),
    const_cast<char*>("-t"),
    const_cast<char*>("elev"),
    const_cast<char*>("input"),
    const_cast<char*>("output")
  };
  const auto args1 = parseCommandLineArgumentsCorrect(argc1, argv1);
  ASSERT_STREQ("input", args1.inputFile.c_str());
  ASSERT_STREQ("output", args1.outputFile.c_str());
  ASSERT_STREQ("elev", args1.elevationTag.c_str());
}

// ____________________________________________________________________________
TEST(UTILTESTS, PointToFloor16) {
  ASSERT_EQ(Point<int16_t>(2, 4), Point<double>(2.235, 4.1435).toFloor16());
  ASSERT_EQ(Point<int16_t>(1, 78), Point<float>(1.935, 78.6899435).toFloor16());
  ASSERT_EQ(Point<int16_t>(-3, -5), Point<double>(-2.235, -4.1435).toFloor16());
  ASSERT_EQ(Point<int16_t>(0, -1), Point<double>(0.363, -0.9357).toFloor16());
}

// ____________________________________________________________________________
TEST(UTILTESTS, PointAddOffsetAndNormalize) {
  const auto point = Point<int16_t>(5, 8);
  ASSERT_EQ(Point<uint16_t>(7, 1), point.addOffsetAndNormalize(Point<int16_t>(2, 2), 9));
  ASSERT_EQ(Point<uint16_t>(6, 0), point.addOffsetAndNormalize(Point<int16_t>(1, 1), 9));
  ASSERT_EQ(Point<uint16_t>(7, 8), point.addOffsetAndNormalize(Point<int16_t>(2, 0), 12));
  ASSERT_EQ(Point<uint16_t>(5, 10), point.addOffsetAndNormalize(Point<int16_t>(0, 2), 12));
  ASSERT_EQ(Point<uint16_t>(7, 10), point.addOffsetAndNormalize(Point<int16_t>(2, 2), 12));
  ASSERT_EQ(Point<uint16_t>(2, 5), point.addOffsetAndNormalize(Point<int16_t>(6, 6), 9));
  ASSERT_EQ(Point<uint16_t>(0, 3), point.addOffsetAndNormalize(Point<int16_t>(-5, -5), 9));

  const auto point1 = Point<int16_t>(0, 0);
  ASSERT_EQ(Point<uint16_t>(2, 2), point1.addOffsetAndNormalize(Point<int16_t>(2, 2), 9));
  ASSERT_EQ(Point<uint16_t>(8, 8), point1.addOffsetAndNormalize(Point<int16_t>(-1, -1), 9));
  ASSERT_EQ(Point<uint16_t>(7, 7), point1.addOffsetAndNormalize(Point<int16_t>(-2, -2), 9));
}

// ____________________________________________________________________________
TEST(UTILTESTS, PointAddOffsetAndNormalizeInPlace) {
  auto point1 = Point<int16_t>(5, 8);
  point1.addOffsetAndNormalizeInPlace(Point<int16_t>(2, 2), 9);
  ASSERT_EQ(Point<int16_t>(7, 1), point1);

  auto point2 = Point<int16_t>(5, 8);
  point2.addOffsetAndNormalizeInPlace(Point<int16_t>(1, 1), 9);
  ASSERT_EQ(Point<int16_t>(6, 0), point2);

  auto point3 = Point<int16_t>(5, 8);
  point3.addOffsetAndNormalizeInPlace(Point<int16_t>(2, 0), 12);
  ASSERT_EQ(Point<int16_t>(7, 8), point3);

  auto point4 = Point<int16_t>(5, 8);
  point4.addOffsetAndNormalizeInPlace(Point<int16_t>(0, 2), 12);
  ASSERT_EQ(Point<int16_t>(5, 10), point4);

  auto point5 = Point<int16_t>(5, 8);
  point5.addOffsetAndNormalizeInPlace(Point<int16_t>(2, 2), 12);
  ASSERT_EQ(Point<int16_t>(7, 10), point5);

  auto point6 = Point<int16_t>(5, 8);
  point6.addOffsetAndNormalizeInPlace(Point<int16_t>(6, 6), 9);
  ASSERT_EQ(Point<int16_t>(2, 5), point6);

  auto point7 = Point<int16_t>(5, 8);
  point7.addOffsetAndNormalizeInPlace(Point<int16_t>(-5, -5), 9);
  ASSERT_EQ(Point<int16_t>(0, 3), point7);

  auto point8 = Point<int16_t>(0, 0);
  point8.addOffsetAndNormalizeInPlace(Point<int16_t>(2, 2), 9);
  ASSERT_EQ(Point<int16_t>(2, 2), point8);

  auto point9 = Point<int16_t>(0, 0);
  point9.addOffsetAndNormalizeInPlace(Point<int16_t>(-1, -1), 9);
  ASSERT_EQ(Point<int16_t>(8, 8), point9);

  auto point10 = Point<int16_t>(0, 0);
  point10.addOffsetAndNormalizeInPlace(Point<int16_t>(-2, -2), 9);
  ASSERT_EQ(Point<int16_t>(7, 7), point10);
}

// ____________________________________________________________________________
TEST(UTILTESTS, Vector3dScalarProduct) {
  const auto v1 = Vector3d<double>(1.5, 4, 7.25);
  const auto v2 = Vector3d<double>(6, 1.75, 3);
  ASSERT_EQ(37.75, v1 * v2);
}

// ____________________________________________________________________________
TEST(UTILTESTS, Vector3dCrossProduct) {
  const auto v1 = Vector3d<double>(1.5, 4, 7.25);
  const auto v2 = Vector3d<double>(6, 1.75, 3);
  ASSERT_EQ(Vector3d<double>(-0.6875, 39, -21.375), v1.crossProduct(v2));
}

// ____________________________________________________________________________
TEST(UTILTESTS, Vector3dNormalAndParallelToXY) {
  const auto v1 = Vector3d<int>(2, 2, 2);
  ASSERT_EQ(Vector3d<int>(-2, 2, 0), v1.normalAndParallelToXY());
}

// ____________________________________________________________________________
TEST(UTILTESTS, GetOsmStats) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser("./testMap.osm", &handler);
  statsParser.parse();
  OsmStats osmStats = handler.getOsmStats();

  ASSERT_EQ((int16_t)7, osmStats.minLon);
  ASSERT_EQ((int16_t)47, osmStats.minLat);
  ASSERT_EQ((int16_t)8, osmStats.maxLon);
  ASSERT_EQ((int16_t)48, osmStats.maxLat);
  ASSERT_EQ((uint64_t)179, osmStats.nodeCount);
  ASSERT_EQ((uint64_t)56, osmStats.wayCount);
  ASSERT_EQ((uint64_t)7, osmStats.relationCount);
  ASSERT_EQ((uint64_t)1, osmStats.min);
  ASSERT_EQ((uint64_t)181, osmStats.max);
}
