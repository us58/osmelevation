// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_PARSER_RELATIONPARSER_H_
#define SRC_PARSER_RELATIONPARSER_H_

#include <string>
#include "parser/OsmHandler.h"
#include "util/osm/OsmStats.h"

namespace parser {

using parser::OsmHandler;
using util::osm::OsmStats;

class RelationParser {
 public:
  // Hand over the NASADEM file handler.
  RelationParser(const std::string& osmFile, OsmHandler* handler,
                 const OsmStats& osmStats);

  void parse();

 private:
  const std::string& _osmFile;

  OsmHandler* _handler;

  const OsmStats& _osmStats;
};

}  // namespace parser

#endif  // SRC_PARSER_RELATIONPARSER_H_
