// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_PARSER_OSMHANDLER_H_
#define SRC_PARSER_OSMHANDLER_H_

#include <cstdint>
#include <tuple>
#include "osmium/handler.hpp"
#include "osmelevation/elevation/GeoElevation.h"


namespace parser {

using osmelevation::elevation::GeoElevation;
using GeoSection = std::tuple<int16_t, int16_t, int16_t, int16_t>;

class OsmHandler : public osmium::handler::Handler {
 public:
  OsmHandler();

  // Number of osm entities already worked off.
  virtual uint64_t getCount() const;

  // Gets called for each node.
  virtual void node(const osmium::Node&) = 0;

  // Gets called for each way.
  virtual void way(const osmium::Way&) = 0;

  // Gets called for each relation.
  virtual void relation(const osmium::Relation&) = 0;

 protected:
  // Number of osm entities worked off.
  uint64_t _count;
};

}  // namespace parser

#endif  // SRC_PARSER_OSMHANDLER_H_
