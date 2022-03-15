// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include "global/Constants.h"
#include "util/index/ElevationIndexSparse.h"

using global::INVALID_ELEV;
using util::index::ElevationIndexSparse;

// ____________________________________________________________________________
TEST(ElevationIndexSparseTest, getElevationSparse) {
  ElevationIndexSparse elevationIndexSparse(3, 3);

  elevationIndexSparse.process();
  ASSERT_EQ(INVALID_ELEV, elevationIndexSparse.getElevation(1));
  ASSERT_EQ(INVALID_ELEV, elevationIndexSparse.getElevation(2));
  ASSERT_EQ(INVALID_ELEV, elevationIndexSparse.getElevation(3));

  elevationIndexSparse.setElevation(1, 50);
  elevationIndexSparse.setElevation(2, -935);
  elevationIndexSparse.setElevation(3, 3553);

  elevationIndexSparse.process();
  ASSERT_EQ(50, elevationIndexSparse.getElevation(1));
  ASSERT_EQ(-935, elevationIndexSparse.getElevation(2));
  ASSERT_EQ(3553, elevationIndexSparse.getElevation(3));

  // If asked for an id that was not added, invalid elevation
  // should be returned.
  ASSERT_EQ(INVALID_ELEV, elevationIndexSparse.getElevation(100));
}
