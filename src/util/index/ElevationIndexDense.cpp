// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <math.h>
#include "global/Constants.h"
#include "util/index/ElevationIndex.h"
#include "util/index/ElevationIndexDense.h"

using global::INVALID_ELEV;
using util::index::ElevationIndex;
using util::index::ElevationIndexDense;
using util::index::getBits;
using util::index::setBits;

// ____________________________________________________________________________
ElevationIndexDense::ElevationIndexDense(const uint64_t count,
                                         const uint64_t max) :
                                         ElevationIndex(count, max) {
  uint64_t numBits = (_max + 1) * 14;
  uint64_t numElements = ceil(static_cast<double>(numBits) / 8);
  _denseIndex.resize(numElements, 0);

  // Set invalid for every entry.
  for (uint64_t i = 0; i < numElements; ++i) {
    setElevation(i, INVALID_ELEV);
  }
}

// ____________________________________________________________________________
void ElevationIndexDense::setElevation(const uint64_t id, int16_t elevation) {
  if (id > _max || id < 1) {
    return;
  }

  // Allow storage of negative elevations.
  elevation += 1000;

  // The position of the most significant bit
  // of the 14-bit integer in the dense array.
  const uint64_t startBitPosArray = id * 14;

  // The position of the byte containing the most
  // significant bit of the 14-bit integer in the dense array.
  const uint64_t startBytePos = startBitPosArray / 8;

  // The position of the least significant bit of the 14-bit
  // integer (LSB) on basis of the least significant bit of the
  // byte at position startBytePos + 2.
  // This can be at most at position 10.
  const uint8_t posLSB = 10 - (startBitPosArray % 8);

  // The bitmask to clear all bits used by the
  // 14-bit integer in startByte, startByte + 1, and startByte + 2.
  const uint8_t bitmask = (posLSB - 4) / 2;

  // The data the least significant byte of the 14-bit integer holds,
  // respectively to the position of the LSB.
  uint8_t byte2 = _denseIndex[startBytePos + 2] & setBits[bitmask][2];
  byte2 |= (elevation << posLSB);

  // The data the byte in the middle,
  // i.e. second least significant byte of the 14-bit integer holds,
  // respectively to the position of the LSB.
  uint8_t byte1 = _denseIndex[startBytePos + 1] & setBits[bitmask][1];
  if (posLSB <= 8) {
    byte1 |= (elevation >> (8 - posLSB));
  } else {
    byte1 |= (elevation << (posLSB - 8));
  }

  // The data the most significant byte of the 14-bit integer holds,
  // respectively to the position of the LSB.
  uint8_t byte0 = _denseIndex[startBytePos] & setBits[bitmask][0];
  byte0 |= (elevation >> (16 - posLSB));

  // Update the data of the least, second least, and most significant byte.
  _denseIndex[startBytePos + 2] = byte2;
  _denseIndex[startBytePos + 1] = byte1;
  _denseIndex[startBytePos] = byte0;
}

// ____________________________________________________________________________
int16_t ElevationIndexDense::getElevation(const uint64_t nodeId) const {
  if (nodeId > _max || nodeId < 1) {
    return INVALID_ELEV;
  }

  // The position of the most significant bit
  // of the 14-bit integer in the dense array.
  const uint64_t startBitPosArray = (nodeId) * 14;

  // The position of the byte containing the most
  // significant bit of the 14-bit integer in the dense array.
  // Hence, the 14-bit integer uses the startByte,
  // startByte + 1, and startByte + 2.
  const uint64_t startBytePos = startBitPosArray / 8;

  // The position of the least significant bit of the 14-bit
  // integer (LSB) on basis of the least significant bit of the
  // byte at position startBytePos + 2.
  // This can be at most at position 10.
  const uint8_t posLSB = 10 - (startBitPosArray % 8);

  // The bitmask to clear all bits not used by the
  // 14-bit integer.
  const uint8_t bitmask = (posLSB - 4) / 2;

  // The data stored in the least, middle, and most significant byte
  // of the 14-bit integer.
  uint8_t byte2 = _denseIndex[startBytePos + 2];
  uint8_t byte1 = _denseIndex[startBytePos + 1];
  uint8_t byte0 = _denseIndex[startBytePos];

  // Extract the elevation from the least, second least,
  // and most significant byte. The bits that do not belong
  // to the 14-bit integer get hidden
  // by the corresponding bitmask from getBits.
  int16_t elevation =
    (((byte0 & getBits[bitmask][0])) << (16 - posLSB)) +
    ((byte2 & getBits[bitmask][2]) >> (posLSB));
  if (posLSB <= 8) {
    elevation += ((byte1 & getBits[bitmask][1]) << (8 - posLSB));
  } else {
    elevation += ((byte1 & getBits[bitmask][1]) >> (posLSB - 8));
  }

  return elevation - 1000;  // Substract 1000 to allow negative elevations.
}
