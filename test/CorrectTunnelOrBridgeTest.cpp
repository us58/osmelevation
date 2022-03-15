// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include <cstdint>
#include <gtest/gtest.h>
#include "global/Constants.h"
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectTunnelOrBridge.h"

using global::INVALID_ELEV;
using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectTunnelOrBridge;

using Routes = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
TEST(CORRECTTUNNELORBRIDGETEST, estimateUnknownSectionFlat) {
  NodeIndex nodeIndex(11);

  // Each node 7 meters apart.
  nodeIndex.setNode(1, 7, 47, 10);
  nodeIndex.setNode(2, 7, 47.0001, 10);
  nodeIndex.setNode(3, 7, 47.0002, 30);
  nodeIndex.setNode(4, 7, 47.0003, 100);
  nodeIndex.setNode(5, 7, 47.0004, 100);
  nodeIndex.setNode(6, 7, 47.0005, 100);
  nodeIndex.setNode(7, 7, 47.0006, 100);
  nodeIndex.setNode(8, 7, 47.0007, 100);
  nodeIndex.setNode(9, 7, 47.0008, 30);
  nodeIndex.setNode(10, 7, 47.0009, 10);
  nodeIndex.setNode(11, 7, 47.001, 10);
  nodeIndex.sort();

  const auto nodeIds = Routes{ { 4, 3, 2, 1 }, { 4, 5, 6, 7, 8 },
                               { 8, 9, 10, 11 } };

  const auto nodeCounts = std::vector<size_t> { 4, 5, 4 };

  AverageElevationIndexSparse elevationIndex(11, 11);

  CorrectTunnelOrBridge tunnel(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  tunnel.buildCoords();
  tunnel.buildElevations();
  tunnel.createElevationPlane();
  tunnel.estimateUnknownSection();

  elevationIndex.process();

  ASSERT_EQ(10, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(10).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(11).elevation);

  ASSERT_EQ(-1000, elevationIndex.getElevation(1));
  ASSERT_EQ(10, elevationIndex.getElevation(2));
  ASSERT_EQ(10, elevationIndex.getElevation(3));
  ASSERT_EQ(10, elevationIndex.getElevation(4));
  ASSERT_EQ(10, elevationIndex.getElevation(5));
  ASSERT_EQ(10, elevationIndex.getElevation(6));
  ASSERT_EQ(10, elevationIndex.getElevation(7));
  ASSERT_EQ(10, elevationIndex.getElevation(8));
  ASSERT_EQ(10, elevationIndex.getElevation(9));
  ASSERT_EQ(10, elevationIndex.getElevation(10));
  ASSERT_EQ(-1000, elevationIndex.getElevation(11));
}

// ____________________________________________________________________________
TEST(CORRECTTUNNELORBRIDGETEST, estimateUnknownSectionSlope) {
  NodeIndex nodeIndex(11);

  nodeIndex.setNode(1, 7, 47, 10);
  nodeIndex.setNode(2, 7, 47.0001, 10);
  nodeIndex.setNode(3, 7, 47.0002, 20);
  nodeIndex.setNode(4, 7, 47.0003, 100);
  nodeIndex.setNode(5, 7, 47.0004, 100);
  nodeIndex.setNode(6, 7, 47.0005, 100);
  nodeIndex.setNode(7, 7, 47.0006, 100);
  nodeIndex.setNode(8, 7, 47.0007, 100);
  nodeIndex.setNode(9, 7, 47.0008, 50);
  nodeIndex.setNode(10, 7, 47.0009, 40);
  nodeIndex.setNode(11, 7, 47.001, 40);
  nodeIndex.sort();

  const auto nodeIds = Routes{ { 4, 3, 2, 1 }, { 4, 5, 6, 7, 8 },
                               { 8, 9, 10, 11 } };

  const auto nodeCounts = std::vector<size_t> { 4, 5, 4 };

  AverageElevationIndexSparse elevationIndex(11, 11);

  CorrectTunnelOrBridge tunnel(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  tunnel.buildCoords();
  tunnel.buildElevations();
  tunnel.createElevationPlane();
  tunnel.estimateUnknownSection();

  elevationIndex.process();

  ASSERT_EQ(10, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(14, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(18, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(21, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(25, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(29, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(33, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(36, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(10).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(11).elevation);

  ASSERT_EQ(-1000, elevationIndex.getElevation(1));
  ASSERT_EQ(10, elevationIndex.getElevation(2));
  ASSERT_EQ(14, elevationIndex.getElevation(3));
  ASSERT_EQ(18, elevationIndex.getElevation(4));
  ASSERT_EQ(21, elevationIndex.getElevation(5));
  ASSERT_EQ(25, elevationIndex.getElevation(6));
  ASSERT_EQ(29, elevationIndex.getElevation(7));
  ASSERT_EQ(33, elevationIndex.getElevation(8));
  ASSERT_EQ(36, elevationIndex.getElevation(9));
  ASSERT_EQ(40, elevationIndex.getElevation(10));
  ASSERT_EQ(-1000, elevationIndex.getElevation(11));
}

// ____________________________________________________________________________
TEST(CORRECTTUNNELORBRIDGETEST, estimateUnknownSectionEndpointRotated) {
  NodeIndex nodeIndex(11);

  nodeIndex.setNode(1, 7, 47, 10);
  nodeIndex.setNode(2, 7, 47.0001, 10);
  nodeIndex.setNode(3, 7, 47.0002, 20);
  nodeIndex.setNode(4, 7, 47.0003, 100);
  nodeIndex.setNode(5, 7, 47.0004, 100);
  nodeIndex.setNode(6, 7, 47.0005, 100);
  nodeIndex.setNode(7, 7, 47.0006, 100);
  nodeIndex.setNode(8, 7, 47.0007, 100);
  nodeIndex.setNode(9, 7, 47.0008, 50);
  nodeIndex.setNode(10, 7, 47.0009, 40);
  nodeIndex.setNode(11, 7, 47.001, 40);
  nodeIndex.sort();

  const auto nodeIds = Routes{ { 8, 9, 10, 11 }, { 8, 7, 6, 5, 4 },
                               { 4, 3, 2, 1 } };

  const auto nodeCounts = std::vector<size_t> { 4, 5, 4 };

  AverageElevationIndexSparse elevationIndex(11, 11);

  CorrectTunnelOrBridge tunnel(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  tunnel.buildCoords();
  tunnel.buildElevations();
  tunnel.createElevationPlane();
  tunnel.estimateUnknownSection();

  elevationIndex.process();

  ASSERT_EQ(10, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(14, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(18, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(21, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(25, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(29, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(33, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(36, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(10).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(11).elevation);

  ASSERT_EQ(-1000, elevationIndex.getElevation(1));
  ASSERT_EQ(10, elevationIndex.getElevation(2));
  ASSERT_EQ(14, elevationIndex.getElevation(3));
  ASSERT_EQ(18, elevationIndex.getElevation(4));
  ASSERT_EQ(21, elevationIndex.getElevation(5));
  ASSERT_EQ(25, elevationIndex.getElevation(6));
  ASSERT_EQ(29, elevationIndex.getElevation(7));
  ASSERT_EQ(33, elevationIndex.getElevation(8));
  ASSERT_EQ(36, elevationIndex.getElevation(9));
  ASSERT_EQ(40, elevationIndex.getElevation(10));
  ASSERT_EQ(-1000, elevationIndex.getElevation(11));
}

// ____________________________________________________________________________
TEST(CORRECTTUNNELORBRIDGETEST, estimateUnknownSectionNonStraight) {
  NodeIndex nodeIndex(11);

  nodeIndex.setNode(1, 1, 4, 100);
  nodeIndex.setNode(2, 2, 4, 110);
  nodeIndex.setNode(3, 3, 4, 150);
  nodeIndex.setNode(4, 3, 5, 200);
  nodeIndex.setNode(5, 4, 5, 250);
  nodeIndex.setNode(6, 5, 4, 200);
  nodeIndex.setNode(7, 6, 5, 170);
  nodeIndex.setNode(8, 7, 5, 150);
  nodeIndex.setNode(9, 7, 4, 140);
  nodeIndex.setNode(10, 8, 4, 80);
  nodeIndex.setNode(11, 9, 4, 50);
  nodeIndex.sort();

  const auto nodeIds = Routes{ { 2, 1 }, { 2, 3, 4, 5, 6, 7, 8, 9, 10 },
                               { 10, 11 } };

  const auto nodeCounts = std::vector<size_t> { 2, 9, 2 };

  AverageElevationIndexSparse elevationIndex(11, 11);

  CorrectTunnelOrBridge tunnel(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  tunnel.buildCoords();
  tunnel.buildElevations();
  tunnel.createElevationPlane();
  tunnel.estimateUnknownSection();

  elevationIndex.process();

  ASSERT_EQ(100, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(94, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(88, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(88, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(81, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(75, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(69, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(63, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(63, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(56, nodeIndex.getNode(10).elevation);
  ASSERT_EQ(50, nodeIndex.getNode(11).elevation);

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(94, elevationIndex.getElevation(2));
  ASSERT_EQ(88, elevationIndex.getElevation(3));
  ASSERT_EQ(88, elevationIndex.getElevation(4));
  ASSERT_EQ(81, elevationIndex.getElevation(5));
  ASSERT_EQ(75, elevationIndex.getElevation(6));
  ASSERT_EQ(69, elevationIndex.getElevation(7));
  ASSERT_EQ(63, elevationIndex.getElevation(8));
  ASSERT_EQ(63, elevationIndex.getElevation(9));
  ASSERT_EQ(56, elevationIndex.getElevation(10));
  ASSERT_EQ(50, elevationIndex.getElevation(11));
}

// ____________________________________________________________________________
TEST(CORRECTTUNNELORBRIDGETEST, estimateUnknownSectionSlopeInvalidInTunnel) {
  NodeIndex nodeIndex(11);

  nodeIndex.setNode(1, 7, 47, 10);
  nodeIndex.setNode(2, 7, 47.0001, 10);
  nodeIndex.setNode(3, 7, 47.0002, 20);
  nodeIndex.setNode(4, 7, 47.0003, INVALID_ELEV);
  nodeIndex.setNode(5, 7, 47.0004, INVALID_ELEV);
  nodeIndex.setNode(6, 7, 47.0005, INVALID_ELEV);
  nodeIndex.setNode(7, 7, 47.0006, INVALID_ELEV);
  nodeIndex.setNode(8, 7, 47.0007, INVALID_ELEV);
  nodeIndex.setNode(9, 7, 47.0008, 50);
  nodeIndex.setNode(10, 7, 47.0009, 40);
  nodeIndex.setNode(11, 7, 47.001, 40);
  nodeIndex.sort();

  const auto nodeIds = Routes{ { 4, 3, 2, 1 }, { 4, 5, 6, 7, 8 },
                               { 8, 9, 10, 11 } };

  const auto nodeCounts = std::vector<size_t> { 4, 5, 4 };

  AverageElevationIndexSparse elevationIndex(11, 11);

  CorrectTunnelOrBridge tunnel(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  tunnel.buildCoords();
  tunnel.buildElevations();
  tunnel.createElevationPlane();
  tunnel.estimateUnknownSection();

  elevationIndex.process();

  ASSERT_EQ(10, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(10, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(14, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(18, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(21, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(25, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(29, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(33, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(36, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(10).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(11).elevation);

  ASSERT_EQ(-1000, elevationIndex.getElevation(1));
  ASSERT_EQ(10, elevationIndex.getElevation(2));
  ASSERT_EQ(14, elevationIndex.getElevation(3));
  ASSERT_EQ(18, elevationIndex.getElevation(4));
  ASSERT_EQ(21, elevationIndex.getElevation(5));
  ASSERT_EQ(25, elevationIndex.getElevation(6));
  ASSERT_EQ(29, elevationIndex.getElevation(7));
  ASSERT_EQ(33, elevationIndex.getElevation(8));
  ASSERT_EQ(36, elevationIndex.getElevation(9));
  ASSERT_EQ(40, elevationIndex.getElevation(10));
  ASSERT_EQ(-1000, elevationIndex.getElevation(11));
}

// ____________________________________________________________________________
TEST(CORRECTTUNNELORBRIDGETEST,
     estimateUnknownSectionSlopeInvalidBeforeTunnel) {
  NodeIndex nodeIndex(11);

  nodeIndex.setNode(1, 7, 47, 10);
  nodeIndex.setNode(2, 7, 47.0001, INVALID_ELEV);
  nodeIndex.setNode(3, 7, 47.0002, 20);
  nodeIndex.setNode(4, 7, 47.0003, 100);
  nodeIndex.setNode(5, 7, 47.0004, 100);
  nodeIndex.setNode(6, 7, 47.0005, 100);
  nodeIndex.setNode(7, 7, 47.0006, 100);
  nodeIndex.setNode(8, 7, 47.0007, 100);
  nodeIndex.setNode(9, 7, 47.0008, 50);
  nodeIndex.setNode(10, 7, 47.0009, 40);
  nodeIndex.setNode(11, 7, 47.001, 40);
  nodeIndex.sort();

  const auto nodeIds = Routes{ { 4, 3, 2, 1 }, { 4, 5, 6, 7, 8 },
                               { 8, 9, 10, 11 } };

  const auto nodeCounts = std::vector<size_t> { 4, 5, 4 };

  AverageElevationIndexSparse elevationIndex(11, 11);

  CorrectTunnelOrBridge tunnel(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  tunnel.buildCoords();
  tunnel.buildElevations();
  tunnel.createElevationPlane();
  tunnel.estimateUnknownSection();

  elevationIndex.process();

  ASSERT_EQ(10, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(INVALID_ELEV, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(20, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(5).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(6).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(7).elevation);
  ASSERT_EQ(100, nodeIndex.getNode(8).elevation);
  ASSERT_EQ(50, nodeIndex.getNode(9).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(10).elevation);
  ASSERT_EQ(40, nodeIndex.getNode(11).elevation);

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
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(11));
}
