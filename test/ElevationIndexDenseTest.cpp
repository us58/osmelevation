// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include "global/Constants.h"
#include "util/index/ElevationIndexDense.h"

using global::INVALID_ELEV;
using util::index::ElevationIndexDense;

// ____________________________________________________________________________
TEST(ElevationIndexDenseTest, getElevationDense) {
  ElevationIndexDense elevationIndexDense(3, 3);
  elevationIndexDense.process();

  ASSERT_EQ(INVALID_ELEV, elevationIndexDense.getElevation(1));
  ASSERT_EQ(INVALID_ELEV, elevationIndexDense.getElevation(2));
  ASSERT_EQ(INVALID_ELEV, elevationIndexDense.getElevation(3));

  elevationIndexDense.setElevation(1, 50);
  elevationIndexDense.setElevation(2, -935);
  elevationIndexDense.setElevation(3, 3553);
  ASSERT_EQ(50, elevationIndexDense.getElevation(1));
  ASSERT_EQ(-935, elevationIndexDense.getElevation(2));
  ASSERT_EQ(3553, elevationIndexDense.getElevation(3));

  elevationIndexDense.setElevation(1, 999);
  elevationIndexDense.setElevation(2, 24);
  elevationIndexDense.setElevation(3, 6432);
  ASSERT_EQ(999, elevationIndexDense.getElevation(1));
  ASSERT_EQ(24, elevationIndexDense.getElevation(2));
  ASSERT_EQ(6432, elevationIndexDense.getElevation(3));

  elevationIndexDense.setElevation(1, INVALID_ELEV);
  ASSERT_EQ(INVALID_ELEV, elevationIndexDense.getElevation(1));
  
  // Just check if the index still works after invalid insert.
  elevationIndexDense.setElevation(1000, 4874);
  ASSERT_EQ(INVALID_ELEV, elevationIndexDense.getElevation(234));
  ASSERT_EQ(24, elevationIndexDense.getElevation(2));
}

// ____________________________________________________________________________
TEST(ElevationIndexDenseTest, getElevationDenseMore) {
  ElevationIndexDense elevationIndexDense(38, 38);
  elevationIndexDense.process();

  elevationIndexDense.setElevation(1, 10);
  elevationIndexDense.setElevation(2, 11);
  elevationIndexDense.setElevation(3, 12);
  elevationIndexDense.setElevation(4, 13);
  elevationIndexDense.setElevation(5, 14);
  elevationIndexDense.setElevation(6, 15);
  elevationIndexDense.setElevation(7, 16);
  elevationIndexDense.setElevation(8, 17);
  elevationIndexDense.setElevation(9, 18);
  elevationIndexDense.setElevation(10, 19);
  elevationIndexDense.setElevation(11, 20);
  elevationIndexDense.setElevation(12, 21);
  elevationIndexDense.setElevation(13, 22);
  elevationIndexDense.setElevation(14, 23);
  elevationIndexDense.setElevation(15, 24);
  elevationIndexDense.setElevation(16, 25);
  elevationIndexDense.setElevation(17, 26);
  elevationIndexDense.setElevation(18, 27);
  elevationIndexDense.setElevation(19, 28);
  elevationIndexDense.setElevation(20, 29);
  elevationIndexDense.setElevation(21, 4560);
  elevationIndexDense.setElevation(22, 4561);
  elevationIndexDense.setElevation(23, 4562);
  elevationIndexDense.setElevation(24, 4563);
  elevationIndexDense.setElevation(25, 4564);
  elevationIndexDense.setElevation(26, 4565);
  elevationIndexDense.setElevation(27, 4566);
  elevationIndexDense.setElevation(28, 4567);
  elevationIndexDense.setElevation(29, 4568);
  elevationIndexDense.setElevation(30, 4569);
  elevationIndexDense.setElevation(31, 0);
  elevationIndexDense.setElevation(32, -436);
  elevationIndexDense.setElevation(33, 8000);
  elevationIndexDense.setElevation(34, 8001);
  elevationIndexDense.setElevation(35, 8002);
  elevationIndexDense.setElevation(36, 8003);
  elevationIndexDense.setElevation(37, 8004);
  elevationIndexDense.setElevation(38, 8005);

  ASSERT_EQ(10, elevationIndexDense.getElevation(1));
  ASSERT_EQ(11, elevationIndexDense.getElevation(2));
  ASSERT_EQ(12, elevationIndexDense.getElevation(3));
  ASSERT_EQ(13, elevationIndexDense.getElevation(4));
  ASSERT_EQ(14, elevationIndexDense.getElevation(5));
  ASSERT_EQ(15, elevationIndexDense.getElevation(6));
  ASSERT_EQ(16, elevationIndexDense.getElevation(7));
  ASSERT_EQ(17, elevationIndexDense.getElevation(8));
  ASSERT_EQ(18, elevationIndexDense.getElevation(9));
  ASSERT_EQ(19, elevationIndexDense.getElevation(10));
  ASSERT_EQ(20, elevationIndexDense.getElevation(11));
  ASSERT_EQ(21, elevationIndexDense.getElevation(12));
  ASSERT_EQ(22, elevationIndexDense.getElevation(13));
  ASSERT_EQ(23, elevationIndexDense.getElevation(14));
  ASSERT_EQ(24, elevationIndexDense.getElevation(15));
  ASSERT_EQ(25, elevationIndexDense.getElevation(16));
  ASSERT_EQ(26, elevationIndexDense.getElevation(17));
  ASSERT_EQ(27, elevationIndexDense.getElevation(18));
  ASSERT_EQ(28, elevationIndexDense.getElevation(19));
  ASSERT_EQ(29, elevationIndexDense.getElevation(20));
  ASSERT_EQ(4560, elevationIndexDense.getElevation(21));
  ASSERT_EQ(4561, elevationIndexDense.getElevation(22));
  ASSERT_EQ(4562, elevationIndexDense.getElevation(23));
  ASSERT_EQ(4563, elevationIndexDense.getElevation(24));
  ASSERT_EQ(4564, elevationIndexDense.getElevation(25));
  ASSERT_EQ(4565, elevationIndexDense.getElevation(26));
  ASSERT_EQ(4566, elevationIndexDense.getElevation(27));
  ASSERT_EQ(4567, elevationIndexDense.getElevation(28));
  ASSERT_EQ(4568, elevationIndexDense.getElevation(29));
  ASSERT_EQ(4569, elevationIndexDense.getElevation(30));
  ASSERT_EQ(0, elevationIndexDense.getElevation(31));
  ASSERT_EQ(-436, elevationIndexDense.getElevation(32));
  ASSERT_EQ(8000, elevationIndexDense.getElevation(33));
  ASSERT_EQ(8001, elevationIndexDense.getElevation(34));
  ASSERT_EQ(8002, elevationIndexDense.getElevation(35));
  ASSERT_EQ(8003, elevationIndexDense.getElevation(36));
  ASSERT_EQ(8004, elevationIndexDense.getElevation(37));
  ASSERT_EQ(8005, elevationIndexDense.getElevation(38));

  elevationIndexDense.setElevation(10, 1353);
  elevationIndexDense.setElevation(11, 1354);
  elevationIndexDense.setElevation(12, 1355);
  elevationIndexDense.setElevation(13, 1356);
  elevationIndexDense.setElevation(14, 1357);
  elevationIndexDense.setElevation(15, 1358);

  ASSERT_EQ(1353, elevationIndexDense.getElevation(10));
  ASSERT_EQ(1354, elevationIndexDense.getElevation(11));
  ASSERT_EQ(1355, elevationIndexDense.getElevation(12));
  ASSERT_EQ(1356, elevationIndexDense.getElevation(13));
  ASSERT_EQ(1357, elevationIndexDense.getElevation(14));
  ASSERT_EQ(1358, elevationIndexDense.getElevation(15));
}
