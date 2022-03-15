// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include "osmelevation/elevation/NasademFileName.h"
#include "util/geo/Point.h"

using osmelevation::elevation::coordPadZeros;
using osmelevation::elevation::convertToNasademNaming;
using osmelevation::elevation::getNasademFilePath;

using pointInt16 = util::geo::Point<int16_t>;

// ____________________________________________________________________________
TEST(NasademFileNameTest, coordPadZeros) {
  ASSERT_EQ("0", coordPadZeros(0, 0));
  ASSERT_EQ("0", coordPadZeros(0, 1));
  ASSERT_EQ("00", coordPadZeros(0, 2));
  ASSERT_EQ("000", coordPadZeros(0, 3));

  ASSERT_EQ("5", coordPadZeros(5, 0));
  ASSERT_EQ("5", coordPadZeros(5, 1));
  ASSERT_EQ("05", coordPadZeros(5, 2));
  ASSERT_EQ("005", coordPadZeros(5, 3));

  ASSERT_EQ("50", coordPadZeros(50, 0));
  ASSERT_EQ("50", coordPadZeros(50, 1));
  ASSERT_EQ("50", coordPadZeros(50, 2));
  ASSERT_EQ("050", coordPadZeros(50, 3));

  ASSERT_EQ("500", coordPadZeros(500, 0));
  ASSERT_EQ("500", coordPadZeros(500, 1));
  ASSERT_EQ("500", coordPadZeros(500, 2));
  ASSERT_EQ("500", coordPadZeros(500, 3));
}

// ____________________________________________________________________________
TEST(NasademFileNameTest, convertToNasademNaming) {
  ASSERT_EQ("n00e000", convertToNasademNaming(pointInt16(0, 0)));

  ASSERT_EQ("n01e001", convertToNasademNaming(pointInt16(1, 1)));
  ASSERT_EQ("n30e030", convertToNasademNaming(pointInt16(30, 30)));
  ASSERT_EQ("n30e130", convertToNasademNaming(pointInt16(130, 30)));
  ASSERT_EQ("n130e130", convertToNasademNaming(pointInt16(130, 130)));

  ASSERT_EQ("s01e001", convertToNasademNaming(pointInt16(1, -1)));
  ASSERT_EQ("s30e030", convertToNasademNaming(pointInt16(30, -30)));
  ASSERT_EQ("s30e130", convertToNasademNaming(pointInt16(130, -30)));
  ASSERT_EQ("s130e130", convertToNasademNaming(pointInt16(130, -130)));

  ASSERT_EQ("s01w001", convertToNasademNaming(pointInt16(-1, -1)));
  ASSERT_EQ("s30w030", convertToNasademNaming(pointInt16(-30, -30)));
  ASSERT_EQ("s30w130", convertToNasademNaming(pointInt16(-130, -30)));
  ASSERT_EQ("s130w130", convertToNasademNaming(pointInt16(-130, -130)));

  ASSERT_EQ("n01w001", convertToNasademNaming(pointInt16(-1, 1)));
  ASSERT_EQ("n30w030", convertToNasademNaming(pointInt16(-30, 30)));
  ASSERT_EQ("n30w130", convertToNasademNaming(pointInt16(-130, 30)));
  ASSERT_EQ("n130w130", convertToNasademNaming(pointInt16(-130, 130)));
}

// ____________________________________________________________________________
TEST(NasademFileNameTest, getNasademFilePath) {
  ASSERT_EQ("./NASADEM_HGT_n95e995.zip", getNasademFilePath("./", "n95e995"));
  ASSERT_EQ("invalid", getNasademFilePath("./", "notExisting"));
}
