// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include "util/geo/Point.h"
#include "global/Constants.h"
#include "osmelevation/elevation/GeoElevation.h"

using osmelevation::elevation::GeoElevation;
using Coordinate = util::geo::Point<double>;
using global::INVALID_ELEV;

// The NASADEM file (n47e007) used for these tests has a sample size of 3601.
// It holds the elevation of 100 meter for every cell, except for the cell
// in the middle, which has the elevation of 300 meters.
// The coordinate 7.5, 47.5 belongs to this cell.

// Additionally, the NASADEM file N47E008 is used which is the neighboring
// NASADEM file to the right.
// It also contains all elevations at 100 meters except for the cell
// at index 3601 with 300 meters.

// ____________________________________________________________________________
TEST(GeoElevationTest, getInterpolatedElevationSameFile) {
  GeoElevation geoElevation("./");

  // Exactly on known point.
  // ASSERT_EQ(300, geoElevation.getInterpolatedElevation(Coordinate(7.5, 47.5)));

  // In a region where the elevation is 100 meters everywhere.
  ASSERT_EQ(100, geoElevation.getInterpolatedElevation(Coordinate(7.1243,
                                                                  47.32521)));

  // Expected behavior when moving to the right
  // from the middle cell (or any other direction):
  // The elevation should decrease from 300 meters when moving away,
  // until it reaches 100 meters.
  ASSERT_EQ(300, geoElevation.getInterpolatedElevation(Coordinate(7.5000001,
                                                                  47.5)));
  ASSERT_EQ(300, geoElevation.getInterpolatedElevation(Coordinate(7.500001,
                                                                  47.5)));
  ASSERT_EQ(299, geoElevation.getInterpolatedElevation(Coordinate(7.50001,
                                                                  47.5)));
  ASSERT_EQ(275, geoElevation.getInterpolatedElevation(Coordinate(7.50005,
                                                                  47.5)));
  ASSERT_EQ(220, geoElevation.getInterpolatedElevation(Coordinate(7.5001,
                                                                  47.5)));
  ASSERT_EQ(122, geoElevation.getInterpolatedElevation(Coordinate(7.5002,
                                                                  47.5)));
  ASSERT_EQ(102, geoElevation.getInterpolatedElevation(Coordinate(7.50025,
                                                                  47.5)));
  ASSERT_EQ(100, geoElevation.getInterpolatedElevation(Coordinate(7.5006,
                                                                  47.5)));

}

// ____________________________________________________________________________
TEST(GeoElevationTest, getInterpolatedElevationTwoFiles) {
  GeoElevation geoElevation("./");

  // When a coordinate originates into one of the cells directly on the
  // sides, a cell that lays in a neighboring NASADEM file is needed.

  // With accessing the right-most cell at the second row, the NASADEM file
  // N47E008 will be requested to access the second-left-most cell at the second row.
  // Since this cell is the only one with the elevation of 300 meters,
  // the result should be more than 100 meters.
  ASSERT_EQ(122, geoElevation.getInterpolatedElevation(Coordinate(7.99999,
                                                                  47.9996)));
}

// ____________________________________________________________________________
TEST(GeoElevationTest, getInterpolatedElevationInvalid) {
  GeoElevation geoElevation("./");

  // For coordinates where no elevation data exists, the invalid
  // elevation -9999 should be returned.
  ASSERT_EQ(INVALID_ELEV,
            geoElevation.getInterpolatedElevation(Coordinate(500.6356,
                                                             245.2435)));
}

// ____________________________________________________________________________
TEST(GeoElevationTest, getInterpolatedElevationInvalidNoElevationHole) {
  GeoElevation geoElevation("./");

  // The N10E010.hgt NASADEM file contains a hole with no elevation in the middle,
  // 100 meters everywhere else. 
  ASSERT_EQ(INVALID_ELEV,
            geoElevation.getInterpolatedElevation(Coordinate(10.5, 10.5)));
  ASSERT_EQ(100,
            geoElevation.getInterpolatedElevation(Coordinate(10.5, 10.4991)));
}
