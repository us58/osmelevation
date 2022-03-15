// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <cstdint>
#include "util/index/ElevationIndex.h"

using util::index::ElevationIndex;

// ____________________________________________________________________________
ElevationIndex::ElevationIndex(const uint64_t count, const uint64_t max) :
                               _count(count), _max(max) {}
