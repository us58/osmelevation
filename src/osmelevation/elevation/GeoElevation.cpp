// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <math.h>
#include "osmelevation/elevation/NasademFile.h"
#include "util/geo/Point.h"
#include "util/geo/Geo.h"
#include "global/Constants.h"
#include "osmelevation/elevation/GeoElevation.h"

using osmelevation::elevation::GeoElevation;
using osmelevation::elevation::NasademFile;
using util::geo::haversineApprox;
using global::INVALID_ELEV;
using CoordInt = util::geo::Point<int16_t>;
using Coordinate = util::geo::Point<double>;
using Cell = util::geo::Point<uint16_t>;

// ____________________________________________________________________________
GeoElevation::GeoElevation(const std::string& nasademDir) :
                           _nasademDir(nasademDir) {}

// ____________________________________________________________________________
int16_t GeoElevation::getInterpolatedElevation(const Coordinate& coord) {
  // Bottom left coordinate of the NASADEM file.
  const CoordInt originCoord = coord.toFloor16();
  const NasademFile& centerNasademFile = getNasademFile(originCoord);
  const Cell centerCell = centerNasademFile.getCellFromCoord(coord);
  const uint16_t samples = centerNasademFile.getSamples();

  // Use inverse distance weighting to get the interpolated elevation.
  double weights = 0.0;
  double elevation = 0.0;

  const int16_t cellElevation =
    centerNasademFile.getElevationFromCell(centerCell);

  if (cellElevation != INVALID_ELEV) {
    // Cell containing the coordinate.
    const auto cellCenter = centerNasademFile.getCellCenter(centerCell);
    if (coord == cellCenter) {
      return centerNasademFile.getElevationFromCell(centerCell);
    }

    const double distance = haversineApprox(cellCenter, coord);
    const double squareDistance = distance * distance;
    weights += 1 / squareDistance;
    elevation += cellElevation / squareDistance;
  } else {
    return INVALID_ELEV;
  }

  // Neighbor cells.
  for (const auto& offset : cellOffsets) {
    // Respectively to the number of samples.
    Cell neighborCell = centerCell.addOffsetAndNormalize(offset, samples);

    // Check if the neighboring cell is inside the center NASADEM file.
    const bool inCenterFile = cellInNasademFile(centerCell, neighborCell,
                                                offset);
    // Get the adjacent NASADEM file if needed.
    const NasademFile& cellNasademFile =
      inCenterFile ? centerNasademFile
      : getNasademFile(originCoord + (offset * Point<int16_t>(1, -1)));

    // If the cell is in a neigbor file, need to go two steps in the
    // offset direction since the NASADEM files are overlapping at
    // all the edges.
    if (!inCenterFile) {
      neighborCell.addOffsetAndNormalizeInPlace(offset, samples);
    }
    const int16_t cellElevation =
      cellNasademFile.getElevationFromCell(neighborCell);

    // Add to inverse distance weighting.
    if (cellElevation != INVALID_ELEV) {
      const Coordinate cellCenter = cellNasademFile.getCellCenter(neighborCell);
      const double distance = haversineApprox(cellCenter, coord);
      const double squareDistance = distance * distance;
      weights += 1 / squareDistance;
      elevation += cellElevation / squareDistance;
    }
  }
  return static_cast<int16_t>(std::lround(elevation / weights));
}

// ____________________________________________________________________________
bool GeoElevation::cellInNasademFile(const Cell& originCell,
                                     const Cell& newCell,
                                     const Point<int16_t>& newOffset) {
  int16_t newX = static_cast<int16_t>(newCell.getX()) - newOffset.getX();
  int16_t newY = static_cast<int16_t>(newCell.getY()) - newOffset.getY();

  return newX == originCell.getX() && newY == originCell.getY();
}

// ____________________________________________________________________________
NasademFile& GeoElevation::getNasademFile(const CoordInt& originCoord) {
  // Convert the coodinate pair to an unique hashable integer.
  const size_t key = coordToKey(originCoord);
  auto nasademFileIt = _nasademFiles.try_emplace(key, _nasademDir,
                                                 originCoord);

  return nasademFileIt.first->second;
}

// ____________________________________________________________________________
int16_t GeoElevation::getElevation(const Coordinate& coord) {
  const CoordInt coordOrigin = coord.toFloor16();
  return getNasademFile(coordOrigin).getElevationFromCoord(coord);
}

// ____________________________________________________________________________
size_t GeoElevation::coordToKey(const Point<int16_t>& point) {
  return static_cast<size_t>(point.getX()) << 32 | (unsigned int)point.getY();
}

// ____________________________________________________________________________
void GeoElevation::clear() {
  _nasademFiles.clear();
}
