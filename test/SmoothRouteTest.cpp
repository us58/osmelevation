// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include <cstdint>
#include <gtest/gtest.h>
#include "global/Constants.h"
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/SmoothRoute.h"

using global::INVALID_ELEV;
using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::SmoothRoute;

using Routes = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
TEST(SMOOTHROUTETEST, smoothMovingAverageFlat) {
  NodeIndex nodeIndex(10);

  nodeIndex.setNode(1, 7, 47, 100);
  nodeIndex.setNode(2, 7.0001, 47, 100);
  nodeIndex.setNode(3, 7.0002, 47, 100);
  nodeIndex.setNode(4, 7.0003, 47, 100);
  nodeIndex.setNode(5, 7.0004, 47, 100);
  nodeIndex.setNode(6, 7.0005, 47, 100);
  nodeIndex.setNode(7, 7.0006, 47, 100);
  nodeIndex.setNode(8, 7.0007, 47, 100);
  nodeIndex.setNode(9, 7.0008, 47, 100);
  nodeIndex.setNode(10, 7.0009, 47, 100);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  const auto nodeCounts = std::vector<size_t> { 10 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  SmoothRoute smoothroute(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  smoothroute.buildCoords();
  smoothroute.buildDistances();
  smoothroute.buildElevations();
  smoothroute.smoothMovingAverage(500);

  elevationIndex.process();

  ASSERT_EQ(100, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(10).elevation);

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
}

// ____________________________________________________________________________
TEST(SMOOTHROUTETEST, smoothMovingAverageSlope) {
  NodeIndex nodeIndex(10);

  // each node 7 meters apart
  nodeIndex.setNode(1, 7, 47, 100);
  nodeIndex.setNode(2, 7.0001, 47, 110);
  nodeIndex.setNode(3, 7.0002, 47, 120);
  nodeIndex.setNode(4, 7.0003, 47, 130);
  nodeIndex.setNode(5, 7.0004, 47, 140);
  nodeIndex.setNode(6, 7.0005, 47, 150);
  nodeIndex.setNode(7, 7.0006, 47, 160);
  nodeIndex.setNode(8, 7.0007, 47, 170);
  nodeIndex.setNode(9, 7.0008, 47, 180);
  nodeIndex.setNode(10, 7.0009, 47, 190);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  const auto nodeCounts = std::vector<size_t> { 10 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  SmoothRoute smoothroute(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  smoothroute.buildCoords();
  smoothroute.buildElevations();
  smoothroute.buildDistances();
  smoothroute.smoothMovingAverage(50);

  elevationIndex.process();

  ASSERT_EQ(100, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(110, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(120, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(130, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(140, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(150, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(160, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(170, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(180, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(190, nodeIndex.getNode(10).elevation);

  ASSERT_EQ(108, elevationIndex.getElevation(1));
  ASSERT_EQ(114, elevationIndex.getElevation(2));
  ASSERT_EQ(121, elevationIndex.getElevation(3));
  ASSERT_EQ(130, elevationIndex.getElevation(4));
  ASSERT_EQ(140, elevationIndex.getElevation(5));
  ASSERT_EQ(150, elevationIndex.getElevation(6));
  ASSERT_EQ(160, elevationIndex.getElevation(7));
  ASSERT_EQ(169, elevationIndex.getElevation(8));
  ASSERT_EQ(176, elevationIndex.getElevation(9));
  ASSERT_EQ(182, elevationIndex.getElevation(10));
}

// ____________________________________________________________________________
TEST(SMOOTHROUTETEST, smoothMovingAverageSlopeDownwards) {
  NodeIndex nodeIndex(10);

  // each node 7 meters apart
  nodeIndex.setNode(1, 7, 47, 190);
  nodeIndex.setNode(2, 7.0001, 47, 180);
  nodeIndex.setNode(3, 7.0002, 47, 170);
  nodeIndex.setNode(4, 7.0003, 47, 160);
  nodeIndex.setNode(5, 7.0004, 47, 150);
  nodeIndex.setNode(6, 7.0005, 47, 140);
  nodeIndex.setNode(7, 7.0006, 47, 130);
  nodeIndex.setNode(8, 7.0007, 47, 120);
  nodeIndex.setNode(9, 7.0008, 47, 110);
  nodeIndex.setNode(10, 7.0009, 47, 100);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  const auto nodeCounts = std::vector<size_t> { 10 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  SmoothRoute smoothroute(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  smoothroute.buildCoords();
  smoothroute.buildElevations();
  smoothroute.buildDistances();
  smoothroute.smoothMovingAverage(50);

  elevationIndex.process();

  ASSERT_EQ(182, elevationIndex.getElevation(1));
  ASSERT_EQ(176, elevationIndex.getElevation(2));
  ASSERT_EQ(169, elevationIndex.getElevation(3));
  ASSERT_EQ(160, elevationIndex.getElevation(4));
  ASSERT_EQ(150, elevationIndex.getElevation(5));
  ASSERT_EQ(140, elevationIndex.getElevation(6));
  ASSERT_EQ(130, elevationIndex.getElevation(7));
  ASSERT_EQ(121, elevationIndex.getElevation(8));
  ASSERT_EQ(114, elevationIndex.getElevation(9));
  ASSERT_EQ(108, elevationIndex.getElevation(10));
}

// ____________________________________________________________________________
TEST(SMOOTHROUTETEST, smoothMovingAverageSlopeWithError) {
  NodeIndex nodeIndex(10);

  // each node 15 meters apart
  nodeIndex.setNode(1, 7, 47, 100);
  nodeIndex.setNode(2, 7.0002, 47, 110);
  nodeIndex.setNode(3, 7.0004, 47, 120);
  nodeIndex.setNode(4, 7.0006, 47, 130);
  nodeIndex.setNode(5, 7.0008, 47, 400);
  nodeIndex.setNode(6, 7.001, 47, 150);
  nodeIndex.setNode(7, 7.0012, 47, 160);
  nodeIndex.setNode(8, 7.0014, 47, 170);
  nodeIndex.setNode(9, 7.0016, 47, 180);
  nodeIndex.setNode(10, 7.0018, 47, 190);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  const auto nodeCounts = std::vector<size_t> { 10 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  SmoothRoute smoothroute(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  smoothroute.buildCoords();
  smoothroute.buildElevations();
  smoothroute.buildDistances();
  smoothroute.smoothMovingAverage(100);

  elevationIndex.process();

  ASSERT_EQ(110, elevationIndex.getElevation(1));
  ASSERT_EQ(144, elevationIndex.getElevation(2));
  ASSERT_EQ(161, elevationIndex.getElevation(3));
  ASSERT_EQ(170, elevationIndex.getElevation(4));
  ASSERT_EQ(179, elevationIndex.getElevation(5));
  ASSERT_EQ(189, elevationIndex.getElevation(6));
  ASSERT_EQ(199, elevationIndex.getElevation(7));
  ASSERT_EQ(198, elevationIndex.getElevation(8));
  ASSERT_EQ(178, elevationIndex.getElevation(9));
  ASSERT_EQ(182, elevationIndex.getElevation(10));
}

// ____________________________________________________________________________
TEST(SMOOTHROUTETEST, smoothMovingAverageSlopeDifferentDistances) {
  NodeIndex nodeIndex(10);

  // distances: 15, 45, 15, 30, 22, 7, 15, 22, 15
  nodeIndex.setNode(1, 7, 47, 100);
  nodeIndex.setNode(2, 7.0002, 47, 110);
  nodeIndex.setNode(3, 7.0008, 47, 120);
  nodeIndex.setNode(4, 7.001, 47, 130);
  nodeIndex.setNode(5, 7.0014, 47, 140);
  nodeIndex.setNode(6, 7.0017, 47, 150);
  nodeIndex.setNode(7, 7.0018, 47, 160);
  nodeIndex.setNode(8, 7.002, 47, 170);
  nodeIndex.setNode(9, 7.0023, 47, 180);
  nodeIndex.setNode(10, 7.0025, 47, 190);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  const auto nodeCounts = std::vector<size_t> { 10 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  SmoothRoute smoothroute(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  smoothroute.buildCoords();
  smoothroute.buildElevations();
  smoothroute.buildDistances();
  smoothroute.smoothMovingAverage(100);

  elevationIndex.process();

  ASSERT_EQ(106, elevationIndex.getElevation(1));
  ASSERT_EQ(109, elevationIndex.getElevation(2));
  ASSERT_EQ(123, elevationIndex.getElevation(3));
  ASSERT_EQ(129, elevationIndex.getElevation(4));
  ASSERT_EQ(143, elevationIndex.getElevation(5));
  ASSERT_EQ(155, elevationIndex.getElevation(6));
  ASSERT_EQ(159, elevationIndex.getElevation(7));
  ASSERT_EQ(167, elevationIndex.getElevation(8));
  ASSERT_EQ(178, elevationIndex.getElevation(9));
  ASSERT_EQ(183, elevationIndex.getElevation(10));
}

// ____________________________________________________________________________
TEST(SMOOTHROUTETEST, smoothMovingAverageSlopeDownwardsInvalidElev) {
  NodeIndex nodeIndex(10);

  // each node 7 meters apart
  nodeIndex.setNode(1, 7, 47, 190);
  nodeIndex.setNode(2, 7.0001, 47, 180);
  nodeIndex.setNode(3, 7.0002, 47, 170);
  nodeIndex.setNode(4, 7.0003, 47, 160);
  nodeIndex.setNode(5, 7.0004, 47, INVALID_ELEV);
  nodeIndex.setNode(6, 7.0005, 47, 140);
  nodeIndex.setNode(7, 7.0006, 47, 130);
  nodeIndex.setNode(8, 7.0007, 47, 120);
  nodeIndex.setNode(9, 7.0008, 47, 110);
  nodeIndex.setNode(10, 7.0009, 47, 100);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };

  const auto nodeCounts = std::vector<size_t> { 10 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  SmoothRoute smoothroute(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  smoothroute.buildCoords();
  smoothroute.buildElevations();
  smoothroute.buildDistances();
  smoothroute.smoothMovingAverage(50);

  elevationIndex.process();

  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(1));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(2));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(3));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(4));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(5));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(6));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(7));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(8));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(9));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(10));
}
