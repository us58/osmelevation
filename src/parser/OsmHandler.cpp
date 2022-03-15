// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <cstdint>
#include "parser/OsmHandler.h"

using parser::OsmHandler;

// _____________________________________________________________________________
OsmHandler::OsmHandler() {
  _count = 0;
}

// _____________________________________________________________________________
uint64_t OsmHandler::getCount() const {
  return _count;
}
