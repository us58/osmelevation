// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <vector>
#include <cstdint>
#include <gtest/gtest.h>
#include "global/Constants.h"
#include "util/index/NodeIndex.h"
#include "util/index/AverageElevationIndexSparse.h"
#include "correctosmelevation/correct/CorrectRiver.h"

using global::INVALID_ELEV;
using util::index::NodeIndex;
using util::index::AverageElevationIndexSparse;
using correctosmelevation::correct::CorrectRiver;

using Routes = std::vector<std::vector<uint64_t>>;

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, oneStreamRiverNoCorrections) {
  NodeIndex nodeIndex(5);

  nodeIndex.setNode(1, 1, 0, 100);
  nodeIndex.setNode(2, 1, 1, 90);
  nodeIndex.setNode(3, 1, 2, 80);
  nodeIndex.setNode(4, 1, 3, 70);
  nodeIndex.setNode(5, 1, 4, 60);
  nodeIndex.sort();

  const auto nodeIds = Routes{ {1, 2, 3, 4, 5} };

  const auto nodeCounts = std::vector<size_t> { 5 };

  AverageElevationIndexSparse elevationIndex(5, 5);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(90, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(80, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(70, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(60, nodeIndex.getNode(5).elevation);

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(90, elevationIndex.getElevation(2));
  ASSERT_EQ(80, elevationIndex.getElevation(3));
  ASSERT_EQ(70, elevationIndex.getElevation(4));
  ASSERT_EQ(60, elevationIndex.getElevation(5));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, oneStreamRiverWithCorrections) {
  NodeIndex nodeIndex(5);

  nodeIndex.setNode(1, 1, 0, 100);
  nodeIndex.setNode(2, 1, 1, 101);
  nodeIndex.setNode(3, 1, 2, 80);
  nodeIndex.setNode(4, 1, 3, 74);
  nodeIndex.setNode(5, 1, 4, 89);
  nodeIndex.sort();

  auto nodeIds = Routes{ {1, 2, 3, 4, 5} };

  auto nodeCounts = std::vector<size_t> { 5 };

  AverageElevationIndexSparse elevationIndex(5, 5);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, nodeIndex.getNode(1).elevation);
  ASSERT_EQ(101, nodeIndex.getNode(2).elevation);
  ASSERT_EQ(80, nodeIndex.getNode(3).elevation);
  ASSERT_EQ(74, nodeIndex.getNode(4).elevation);
  ASSERT_EQ(89, nodeIndex.getNode(5).elevation);

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(100, elevationIndex.getElevation(2));
  ASSERT_EQ(80, elevationIndex.getElevation(3));
  ASSERT_EQ(74, elevationIndex.getElevation(4));
  ASSERT_EQ(74, elevationIndex.getElevation(5));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, twoStreamRiverNoCorrections) {
  NodeIndex nodeIndex(10);

  nodeIndex.setNode(1, 1, 0, 100);
  nodeIndex.setNode(2, 1, 1, 90);
  nodeIndex.setNode(3, 1, 2, 80);
  nodeIndex.setNode(4, 1, 3, 70);
  nodeIndex.setNode(5, 1, 4, 70);
  nodeIndex.setNode(6, 2, 4, 60);
  nodeIndex.setNode(7, 3, 4, 55);
  nodeIndex.setNode(8, 4, 4, 50);
  
  nodeIndex.sort();

  auto nodeIds = Routes{ {1, 2, 3, 4, 5}, { 6, 7, 8 } };

  auto nodeCounts = std::vector<size_t> { 5, 3 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(90, elevationIndex.getElevation(2));
  ASSERT_EQ(80, elevationIndex.getElevation(3));
  ASSERT_EQ(70, elevationIndex.getElevation(4));
  ASSERT_EQ(70, elevationIndex.getElevation(5));
  ASSERT_EQ(60, elevationIndex.getElevation(6));
  ASSERT_EQ(55, elevationIndex.getElevation(7));
  ASSERT_EQ(50, elevationIndex.getElevation(8));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, twoStreamRiverWithCorrections) {
  NodeIndex nodeIndex(10);

  nodeIndex.setNode(1, 1, 0, 100);
  nodeIndex.setNode(2, 1, 1, 92);
  nodeIndex.setNode(3, 1, 2, 95);
  nodeIndex.setNode(4, 1, 3, 100);
  nodeIndex.setNode(5, 1, 4, 90);
  nodeIndex.setNode(6, 1, 5, 60);
  nodeIndex.setNode(7, 2, 4, 92);
  nodeIndex.setNode(8, 3, 4, 70);
  
  nodeIndex.sort();

  auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6}, { 5, 7, 8 } };

  auto nodeCounts = std::vector<size_t> { 6, 3 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(92, elevationIndex.getElevation(2));
  ASSERT_EQ(92, elevationIndex.getElevation(3));
  ASSERT_EQ(92, elevationIndex.getElevation(4));
  ASSERT_EQ(90, elevationIndex.getElevation(5));
  ASSERT_EQ(60, elevationIndex.getElevation(6));
  ASSERT_EQ(90, elevationIndex.getElevation(7));
  ASSERT_EQ(70, elevationIndex.getElevation(8));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, oneStreamRiverWithSideStream) {
  NodeIndex nodeIndex(10);

  nodeIndex.setNode(1, 1, 0, 100);
  nodeIndex.setNode(2, 1, 1, 90);
  nodeIndex.setNode(3, 1, 2, 80);
  nodeIndex.setNode(4, 1, 3, 70);
  nodeIndex.setNode(5, 1, 4, 60);
  nodeIndex.setNode(6, 1, 5, 50);
  nodeIndex.setNode(7, 2, 2, 60);
  
  nodeIndex.sort();

  auto nodeIds = Routes{ {1, 2, 3, 4, 5, 6}, { 2, 7, 4 } };

  auto nodeCounts = std::vector<size_t> { 6, 3 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(90, elevationIndex.getElevation(2));
  ASSERT_EQ(80, elevationIndex.getElevation(3));
  ASSERT_EQ(60, elevationIndex.getElevation(4));
  ASSERT_EQ(60, elevationIndex.getElevation(5));
  ASSERT_EQ(50, elevationIndex.getElevation(6));
  ASSERT_EQ(60, elevationIndex.getElevation(7));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, twoStreamRiverWithConnection) {
  NodeIndex nodeIndex(10);

  nodeIndex.setNode(1, 2, 0, 100);
  nodeIndex.setNode(2, 1, 2, 90);
  nodeIndex.setNode(3, 1, 4, 80);
  nodeIndex.setNode(4, 1, 6, 70);
  nodeIndex.setNode(5, 3, 2, 80);
  nodeIndex.setNode(6, 3, 4, 70);
  nodeIndex.setNode(7, 3, 6, 60);
  nodeIndex.setNode(8, 2, 4, 10);

  nodeIndex.sort();

  auto nodeIds = Routes{ {1, 2, 3, 4 }, { 1, 5, 6, 7 }, { 3, 8, 6 } };

  auto nodeCounts = std::vector<size_t> { 4, 4, 3 };

  AverageElevationIndexSparse elevationIndex(10, 10);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(90, elevationIndex.getElevation(2));
  ASSERT_EQ(80, elevationIndex.getElevation(3));
  ASSERT_EQ(70, elevationIndex.getElevation(4));
  ASSERT_EQ(80, elevationIndex.getElevation(5));
  ASSERT_EQ(10, elevationIndex.getElevation(6));
  ASSERT_EQ(10, elevationIndex.getElevation(7));
  ASSERT_EQ(10, elevationIndex.getElevation(8));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, complexRiverwithInterconnects) {
  NodeIndex nodeIndex(15);

  nodeIndex.setNode(1, 7, 0, 100);
  nodeIndex.setNode(2, 6, 1, 98);
  nodeIndex.setNode(3, 5, 4, 100);
  nodeIndex.setNode(4, 3, 8, 95);
  nodeIndex.setNode(5, 2, 10, 90);
  nodeIndex.setNode(6, 8, 1, 95);
  nodeIndex.setNode(7, 10, 5, 92);
  nodeIndex.setNode(8, 11, 7, 90);
  nodeIndex.setNode(9, 12, 10, 100);
  nodeIndex.setNode(10, 7, 5, 70);
  nodeIndex.setNode(11, 9, 6, 40);
  nodeIndex.setNode(12, 5, 7, 50);
  nodeIndex.setNode(13, 4, 6, 98);
  nodeIndex.setNode(14, 9, 5, 50);
  nodeIndex.setNode(15, 6, 4, 98);

  nodeIndex.sort();

  auto nodeIds = Routes{ {1, 2, 3, 13, 4, 5 }, { 1, 6, 7, 8, 9 },
                         { 3, 15, 10, 11, 8 }, { 11, 12, 4 }, { 10, 14, 7 } };

  auto nodeCounts = std::vector<size_t> { 6, 5, 5, 3, 3 };

  AverageElevationIndexSparse elevationIndex(16, 16);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

  elevationIndex.process();

  ASSERT_EQ(100, elevationIndex.getElevation(1));
  ASSERT_EQ(98, elevationIndex.getElevation(2));
  ASSERT_EQ(98, elevationIndex.getElevation(3));
  ASSERT_EQ(40, elevationIndex.getElevation(4));
  ASSERT_EQ(40, elevationIndex.getElevation(5));
  ASSERT_EQ(95, elevationIndex.getElevation(6));
  ASSERT_EQ(50, elevationIndex.getElevation(7));
  ASSERT_EQ(40, elevationIndex.getElevation(8));
  ASSERT_EQ(40, elevationIndex.getElevation(9));
  ASSERT_EQ(70, elevationIndex.getElevation(10));
  ASSERT_EQ(40, elevationIndex.getElevation(11));
  ASSERT_EQ(40, elevationIndex.getElevation(12));
  ASSERT_EQ(98, elevationIndex.getElevation(13));
  ASSERT_EQ(50, elevationIndex.getElevation(14));
  ASSERT_EQ(98, elevationIndex.getElevation(15));
}

// ____________________________________________________________________________
TEST(CORRECTRIVERTEST, invalidElevation) {
  NodeIndex nodeIndex(15);

  nodeIndex.setNode(1, 7, 0, 100);
  nodeIndex.setNode(2, 6, 1, 98);
  nodeIndex.setNode(3, 5, 4, 100);
  nodeIndex.setNode(4, 3, 8, 95);
  nodeIndex.setNode(5, 2, 10, 90);
  nodeIndex.setNode(6, 8, 1, INVALID_ELEV);
  nodeIndex.setNode(7, 10, 5, 92);
  nodeIndex.setNode(8, 11, 7, 90);
  nodeIndex.setNode(9, 12, 10, 100);
  nodeIndex.setNode(10, 7, 5, 70);
  nodeIndex.setNode(11, 9, 6, 40);
  nodeIndex.setNode(12, 5, 7, 50);
  nodeIndex.setNode(13, 4, 6, 98);
  nodeIndex.setNode(14, 9, 5, 50);
  nodeIndex.setNode(15, 6, 4, 98);

  nodeIndex.sort();

  auto nodeIds = Routes{ { 1, 2, 3, 13, 4, 5 }, { 1, 6, 7, 8, 9 },
                         { 3, 15, 10, 11, 8 }, { 11, 12, 4 }, { 10, 14, 7 } };

  auto nodeCounts = std::vector<size_t> { 6, 5, 5, 3, 3 };

  AverageElevationIndexSparse elevationIndex(16, 16);

  CorrectRiver correctRiver(nodeIds, nodeCounts, nodeIndex, elevationIndex);

  correctRiver.buildElevations();
  correctRiver.buildRiverNodes();
  correctRiver.correctDownstreamElevation();
  correctRiver.updateElevationIndex();

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
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(11));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(12));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(13));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(14));
  ASSERT_EQ(INVALID_ELEV, elevationIndex.getElevation(15));
}
