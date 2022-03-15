// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include <cstdint>
#include "global/Constants.h"
#include "util/index/AverageElevationIndexSparse.h"

using global::INVALID_ELEV;
using util::index::AverageElevationIndexSparse;

// ____________________________________________________________________________
TEST(AverageElevationIndexSparseTest, setAndGetElevation) {
  AverageElevationIndexSparse averageElevationIndexSparse(20, 20);

  averageElevationIndexSparse.process();
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(3));

  averageElevationIndexSparse.setElevation(1, (int16_t)50);
  averageElevationIndexSparse.setElevation(2, (int16_t)-935);
  averageElevationIndexSparse.setElevation(3, (int16_t)3553);
  averageElevationIndexSparse.setElevationTunnelOrBridge(11, 100);

  averageElevationIndexSparse.process();
  ASSERT_EQ(50, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(-935, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(3553, averageElevationIndexSparse.getElevation(3));
  ASSERT_EQ(100, averageElevationIndexSparse.getElevation(11));

  averageElevationIndexSparse.setElevation(1, (int16_t)100);
  averageElevationIndexSparse.setElevation(1, (int16_t)150);
  averageElevationIndexSparse.setElevation(2, (int16_t)-500);
  averageElevationIndexSparse.setElevation(4, (int16_t)1200);
  averageElevationIndexSparse.setElevation(11, (int16_t)500);

  averageElevationIndexSparse.process();
  ASSERT_EQ(100, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(-718, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(3553, averageElevationIndexSparse.getElevation(3));
  ASSERT_EQ(1200, averageElevationIndexSparse.getElevation(4));
  ASSERT_EQ(100, averageElevationIndexSparse.getElevation(11));

  averageElevationIndexSparse.setElevation(1, (int16_t)200);
  averageElevationIndexSparse.setElevation(4, (int16_t)1300);

  averageElevationIndexSparse.process();
  ASSERT_EQ(125, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(-718, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(3553, averageElevationIndexSparse.getElevation(3));
  ASSERT_EQ(1250, averageElevationIndexSparse.getElevation(4));
  ASSERT_EQ(100, averageElevationIndexSparse.getElevation(11));

  averageElevationIndexSparse.setElevation(5, (int16_t)2900);
  averageElevationIndexSparse.setElevationTunnelOrBridge(11, 500);

  averageElevationIndexSparse.process();
  ASSERT_EQ(125, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(-718, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(3553, averageElevationIndexSparse.getElevation(3));
  ASSERT_EQ(1250, averageElevationIndexSparse.getElevation(4));
  ASSERT_EQ(2900, averageElevationIndexSparse.getElevation(5));
  ASSERT_EQ(500, averageElevationIndexSparse.getElevation(11));

  // If asked for an id that was not added, invalid elevation
  // should be returned.
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(100));
}

// ____________________________________________________________________________
TEST(AverageElevationIndexSparseTest, tunnelAndBridge) {
  AverageElevationIndexSparse averageElevationIndexSparse(20, 20);

  averageElevationIndexSparse.process();

  averageElevationIndexSparse.setElevationTunnelOrBridge(1, 200);
  averageElevationIndexSparse.setElevation(1, (int16_t)70);
  averageElevationIndexSparse.process();
  ASSERT_EQ(200, averageElevationIndexSparse.getElevation(1));

  averageElevationIndexSparse.setElevation(1, (int16_t)50);

  averageElevationIndexSparse.process();
  ASSERT_EQ(200, averageElevationIndexSparse.getElevation(1));

  averageElevationIndexSparse.setElevationTunnelOrBridge(1, 300);

  averageElevationIndexSparse.process();
  ASSERT_EQ(300, averageElevationIndexSparse.getElevation(1));

  averageElevationIndexSparse.setElevation(1, (int16_t)190);

  averageElevationIndexSparse.process();
  ASSERT_EQ(300, averageElevationIndexSparse.getElevation(1));

  averageElevationIndexSparse.setElevation(1, (int16_t)19);
  averageElevationIndexSparse.setElevation(1, (int16_t)1910);
  averageElevationIndexSparse.setElevation(1, (int16_t)13);
  averageElevationIndexSparse.setElevation(1, (int16_t)1490);
  averageElevationIndexSparse.process();
  ASSERT_EQ(300, averageElevationIndexSparse.getElevation(1));

  averageElevationIndexSparse.setElevationTunnelOrBridge(1, 500);
  averageElevationIndexSparse.setElevationTunnelOrBridge(1, 200);
  averageElevationIndexSparse.setElevationTunnelOrBridge(1, 700);
  averageElevationIndexSparse.setElevation(1, (int16_t)643);
  averageElevationIndexSparse.setElevation(1, (int16_t)2446);
  averageElevationIndexSparse.setElevation(1, (int16_t)12355);
  averageElevationIndexSparse.process();
  ASSERT_EQ(700, averageElevationIndexSparse.getElevation(1));
}

// ____________________________________________________________________________
TEST(AverageElevationIndexSparseTest, invalidIds) {
  AverageElevationIndexSparse averageElevationIndexSparse(5, 5);

  averageElevationIndexSparse.process();
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(-1));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(0));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(3));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(4));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(5));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(6));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(7));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(8472859));

  averageElevationIndexSparse.setElevation(1, (int16_t)10);
  averageElevationIndexSparse.setElevation(2, (int16_t)20);
  averageElevationIndexSparse.setElevation(3, (int16_t)30);
  averageElevationIndexSparse.setElevation(4, (int16_t)40);
  averageElevationIndexSparse.setElevation(5, (int16_t)50);

  averageElevationIndexSparse.process();
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(-1));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(0));
  ASSERT_EQ(10, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(20, averageElevationIndexSparse.getElevation(2));
  ASSERT_EQ(30, averageElevationIndexSparse.getElevation(3));
  ASSERT_EQ(40, averageElevationIndexSparse.getElevation(4));
  ASSERT_EQ(50, averageElevationIndexSparse.getElevation(5));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(6));
  ASSERT_EQ(INVALID_ELEV, averageElevationIndexSparse.getElevation(7));

  averageElevationIndexSparse.setElevation(1, (int16_t)10);
  averageElevationIndexSparse.setElevation(2, (int16_t)20);
  averageElevationIndexSparse.setElevation(3, (int16_t)30);
  averageElevationIndexSparse.setElevation(4, (int16_t)40);
  averageElevationIndexSparse.setElevation(5, (int16_t)50);
}

// ____________________________________________________________________________
TEST(AverageElevationIndexSparseTest, floats) {
  AverageElevationIndexSparse averageElevationIndexSparse(5, 5);

  averageElevationIndexSparse.process();

  averageElevationIndexSparse.setElevation(1, (float)0.5);
  averageElevationIndexSparse.setElevation(1, (float)1.5);

  averageElevationIndexSparse.process();
  ASSERT_EQ(1, averageElevationIndexSparse.getElevation(1));

  averageElevationIndexSparse.setElevation(2, (float)0.5);
  averageElevationIndexSparse.setElevation(2, (float)1);

  averageElevationIndexSparse.process();
  ASSERT_EQ(1, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(1, averageElevationIndexSparse.getElevation(2));

  averageElevationIndexSparse.setElevation(2, (int16_t)3);

  averageElevationIndexSparse.process();
  ASSERT_EQ(1, averageElevationIndexSparse.getElevation(1));
  ASSERT_EQ(2, averageElevationIndexSparse.getElevation(2));
}
