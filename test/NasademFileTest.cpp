// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <gtest/gtest.h>
#include "osmelevation/elevation/NasademFile.h"
#include "global/Constants.h"
#include "util/geo/Point.h"

using osmelevation::elevation::NasademFile;
using global::INVALID_ELEV;
using CoordInt = util::geo::Point<int16_t>;
using Coordinate = util::geo::Point<double>;
using Cell = util::geo::Point<uint16_t>;

// [0     -1  -2000, 3,     4,
//  5,    6,  20000, 8932, -9,
//  -400, 11, 12,    13,   14,
//  15,   16, 17,    18,   19,
//  20,   21, 22,    23,   24]
const uint8_t data[] = { 0b00000000, 0b00000000, 0b11111111, 0b11111111,
                         0b11111000, 0b00110000, 0b00000000, 0b00000011,
                         0b00000000, 0b00000100, 0b00000000, 0b00000101,
                         0b00000000, 0b00000110, 0b01001110, 0b00100000,
                         0b00100010, 0b11100100, 0b11111111, 0b11110111,
                         0b11111110, 0b01110000, 0b00000000, 0b00001011,
                         0b00000000, 0b00001100, 0b00000000, 0b00001101,
                         0b00000000, 0b00001110, 0b00000000, 0b00001111,
                         0b00000000, 0b00010000, 0b00000000, 0b00010001,
                         0b00000000, 0b00010010, 0b00000000, 0b00010011,
                         0b00000000, 0b00010100, 0b00000000, 0b00010101,
                         0b00000000, 0b00010110, 0b00000000, 0b00010111,
                         0b00000000, 0b00011000 };

// Expected behavior: The top row and right collumn should not be accessible
// as they belong to the adjacent tiles.

// ____________________________________________________________________________
TEST(NasademFileTest, constructorFileExists) {
  NasademFile file("./", CoordInt(995, 95));

  ASSERT_EQ(995, file.getLon());
  ASSERT_EQ(95, file.getLat());
  ASSERT_EQ(5, file.getSamples());
  ASSERT_EQ(true, file.exists());
}

// ____________________________________________________________________________
TEST(NasademFileTest, constructorFileExistsNot) {
  NasademFile file("./", CoordInt(1, 2));

  ASSERT_EQ(1, file.getLon());
  ASSERT_EQ(2, file.getLat());
  ASSERT_EQ(1, file.getSamples());
  ASSERT_EQ(false, file.exists());

  ASSERT_DOUBLE_EQ(1, file.getCellCenter(Cell(0, 0)).getX());
  ASSERT_DOUBLE_EQ(3, file.getCellCenter(Cell(0, 0)).getY());

  ASSERT_EQ(Cell(0, 0), file.getCellFromCoord(Coordinate(1, 2)));
  ASSERT_EQ(INVALID_ELEV, file.getElevationFromCell(Cell(0, 0)));
  ASSERT_EQ(INVALID_ELEV, file.getElevationFromCell(Cell(0, 1)));
  ASSERT_EQ(INVALID_ELEV, file.getElevationFromCell(Cell(335, 2435)));
}

// ____________________________________________________________________________
TEST(NasademFileTest, getCellCenter) {
  NasademFile file("./", CoordInt(995, 95));

  // Corners
  ASSERT_DOUBLE_EQ(995, file.getCellCenter(Cell(0, 0)).getX());
  ASSERT_DOUBLE_EQ(96, file.getCellCenter(Cell(0, 0)).getY());

  ASSERT_DOUBLE_EQ(996, file.getCellCenter(Cell(4, 0)).getX());
  ASSERT_DOUBLE_EQ(96, file.getCellCenter(Cell(4, 0)).getY());

  ASSERT_DOUBLE_EQ(996, file.getCellCenter(Cell(4, 4)).getX());
  ASSERT_DOUBLE_EQ(95, file.getCellCenter(Cell(4, 4)).getY());

  ASSERT_DOUBLE_EQ(995, file.getCellCenter(Cell(0, 4)).getX());
  ASSERT_DOUBLE_EQ(95, file.getCellCenter(Cell(0, 4)).getY());

  // Some inner tests
  ASSERT_DOUBLE_EQ(995.25, file.getCellCenter(Cell(1, 1)).getX());
  ASSERT_DOUBLE_EQ(95.75, file.getCellCenter(Cell(1, 1)).getY());

  ASSERT_DOUBLE_EQ(995.75, file.getCellCenter(Cell(3, 4)).getX());
  ASSERT_DOUBLE_EQ(95, file.getCellCenter(Cell(3, 4)).getY());

  ASSERT_DOUBLE_EQ(995, file.getCellCenter(Cell(0, 2)).getX());
  ASSERT_DOUBLE_EQ(95.5, file.getCellCenter(Cell(0, 2)).getY());

  ASSERT_DOUBLE_EQ(995, file.getCellCenter(Cell(0, 1)).getX());
  ASSERT_DOUBLE_EQ(95.75, file.getCellCenter(Cell(0, 1)).getY());

  ASSERT_DOUBLE_EQ(995, file.getCellCenter(Cell(0, 3)).getX());
  ASSERT_DOUBLE_EQ(95.25, file.getCellCenter(Cell(0, 3)).getY());

  ASSERT_DOUBLE_EQ(995.5, file.getCellCenter(Cell(2, 3)).getX());
  ASSERT_DOUBLE_EQ(95.25, file.getCellCenter(Cell(2, 3)).getY());

  ASSERT_DOUBLE_EQ(995.25, file.getCellCenter(Cell(1, 3)).getX());
  ASSERT_DOUBLE_EQ(95.25, file.getCellCenter(Cell(1, 3)).getY());
}

// ____________________________________________________________________________
TEST(NasademFileTest, getCellFromCoord) {
  NasademFile file("./", CoordInt(995, 95));

  // Edges of NASADEM file (Corners and edges of border)
  ASSERT_EQ(Cell(0, 0), file.getCellFromCoord(Coordinate(995.0, 95.999)));
  ASSERT_EQ(Cell(4, 0), file.getCellFromCoord(Coordinate(995.999, 95.995)));
  ASSERT_EQ(Cell(4, 4), file.getCellFromCoord(Coordinate(995.999, 95.0)));
  ASSERT_EQ(Cell(0, 4), file.getCellFromCoord(Coordinate(995.0, 95.0)));

  ASSERT_EQ(Cell(2, 0), file.getCellFromCoord(Coordinate(995.5, 95.999)));
  ASSERT_EQ(Cell(4, 2), file.getCellFromCoord(Coordinate(995.999, 95.5)));
  ASSERT_EQ(Cell(2, 4), file.getCellFromCoord(Coordinate(995.5, 95.0)));
  ASSERT_EQ(Cell(0, 2), file.getCellFromCoord(Coordinate(995.0, 95.5)));

  // Check normal inner behavior.
  ASSERT_EQ(Cell(1, 1), file.getCellFromCoord(Coordinate(995.3, 95.7)));
  ASSERT_EQ(Cell(2, 3), file.getCellFromCoord(Coordinate(995.5, 95.3)));
  ASSERT_EQ(Cell(1, 3), file.getCellFromCoord(Coordinate(995.2, 95.2)));
  ASSERT_EQ(Cell(0, 4), file.getCellFromCoord(Coordinate(995.1, 95.1)));
  ASSERT_EQ(Cell(1, 4), file.getCellFromCoord(Coordinate(995.2, 95.1)));
  ASSERT_EQ(Cell(1, 4), file.getCellFromCoord(Coordinate(995.3, 95.1)));

  ASSERT_EQ(Cell(0, 4), file.getCellFromCoord(Coordinate(995.1, 95.1)));
  ASSERT_EQ(Cell(0, 3), file.getCellFromCoord(Coordinate(995.1, 95.2)));
  ASSERT_EQ(Cell(0, 3), file.getCellFromCoord(Coordinate(995.1, 95.3)));

  // Check the corners regarding to one cell.
  ASSERT_EQ(Cell(3, 1), file.getCellFromCoord(Coordinate(995.6251, 95.6251)));
  ASSERT_EQ(Cell(2, 2), file.getCellFromCoord(Coordinate(995.3751, 95.3751)));
  ASSERT_EQ(Cell(3, 3), file.getCellFromCoord(Coordinate(995.6251, 95.1251)));
  ASSERT_EQ(Cell(1, 2), file.getCellFromCoord(Coordinate(995.1251, 95.3751)));

  // Shift minimally to the left.
  ASSERT_EQ(Cell(2, 1), file.getCellFromCoord(Coordinate(995.6249, 95.6251)));
  ASSERT_EQ(Cell(1, 2), file.getCellFromCoord(Coordinate(995.3749, 95.3751)));
  ASSERT_EQ(Cell(2, 3), file.getCellFromCoord(Coordinate(995.6249, 95.1251)));
  ASSERT_EQ(Cell(0, 2), file.getCellFromCoord(Coordinate(995.1249, 95.3751)));

  // Shift minimally to the bottom.
  ASSERT_EQ(Cell(3, 2), file.getCellFromCoord(Coordinate(995.6251, 95.6249)));
  ASSERT_EQ(Cell(2, 3), file.getCellFromCoord(Coordinate(995.3751, 95.3749)));
  ASSERT_EQ(Cell(3, 4), file.getCellFromCoord(Coordinate(995.6251, 95.1249)));
  ASSERT_EQ(Cell(1, 3), file.getCellFromCoord(Coordinate(995.1251, 95.3749)));

  // Shift minimally to the left and bottom.
  ASSERT_EQ(Cell(2, 2), file.getCellFromCoord(Coordinate(995.6249, 95.6249)));
  ASSERT_EQ(Cell(1, 3), file.getCellFromCoord(Coordinate(995.3749, 95.3749)));
  ASSERT_EQ(Cell(2, 4), file.getCellFromCoord(Coordinate(995.6249, 95.1249)));
  ASSERT_EQ(Cell(0, 3), file.getCellFromCoord(Coordinate(995.1249, 95.3749)));
}

// ____________________________________________________________________________
TEST(NasademFileTest, getElevationFromCell) {
  NasademFile file("./", CoordInt(995, 95));

  // Check every cell (also checks the private getData method)
  ASSERT_EQ(0, file.getElevationFromCell(Cell(0, 0)));
  ASSERT_EQ(-1, file.getElevationFromCell(Cell(1, 0)));
  ASSERT_EQ(INVALID_ELEV, file.getElevationFromCell(Cell(2, 0)));
  ASSERT_EQ(3, file.getElevationFromCell(Cell(3, 0)));
  ASSERT_EQ(4, file.getElevationFromCell(Cell(4, 0)));
  ASSERT_EQ(5, file.getElevationFromCell(Cell(0, 1)));
  ASSERT_EQ(6, file.getElevationFromCell(Cell(1, 1)));
  ASSERT_EQ(INVALID_ELEV, file.getElevationFromCell(Cell(2, 1)));
  ASSERT_EQ(8932, file.getElevationFromCell(Cell(3, 1)));
  ASSERT_EQ(-9, file.getElevationFromCell(Cell(4, 1)));
  ASSERT_EQ(-400, file.getElevationFromCell(Cell(0, 2)));
  ASSERT_EQ(11, file.getElevationFromCell(Cell(1, 2)));
  ASSERT_EQ(12, file.getElevationFromCell(Cell(2, 2)));
  ASSERT_EQ(13, file.getElevationFromCell(Cell(3, 2)));
  ASSERT_EQ(14, file.getElevationFromCell(Cell(4, 2)));
  ASSERT_EQ(15, file.getElevationFromCell(Cell(0, 3)));
  ASSERT_EQ(16, file.getElevationFromCell(Cell(1, 3)));
  ASSERT_EQ(17, file.getElevationFromCell(Cell(2, 3)));
  ASSERT_EQ(18, file.getElevationFromCell(Cell(3, 3)));
  ASSERT_EQ(19, file.getElevationFromCell(Cell(4, 3)));
  ASSERT_EQ(20, file.getElevationFromCell(Cell(0, 4)));
  ASSERT_EQ(21, file.getElevationFromCell(Cell(1, 4)));
  ASSERT_EQ(22, file.getElevationFromCell(Cell(2, 4)));
  ASSERT_EQ(23, file.getElevationFromCell(Cell(3, 4)));
  ASSERT_EQ(24, file.getElevationFromCell(Cell(4, 4)));
}

// ____________________________________________________________________________
TEST(NasademFileTest, getElevationFromCoord) {
  NasademFile file("./", CoordInt(995, 95));

  // Already covered by getCellFromCoord and getElevationFromCell, just test
  // basic calling.

  ASSERT_EQ(13, file.getElevationFromCoord(Coordinate(995.7, 95.5)));
}
