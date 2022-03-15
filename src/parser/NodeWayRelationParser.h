// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_PARSER_NODEWAYRELATIONPARSER_H_
#define SRC_PARSER_NODEWAYRELATIONPARSER_H_

#include <string>
#include "parser/OsmHandler.h"

namespace parser {

class NodeWayRelationParser {
 public:
  NodeWayRelationParser(const std::string& osmFile, OsmHandler* handler);

  void parse();

 private:
  const std::string& _osmFile;

  OsmHandler* _handler;
};

}  // namespace parser

#endif  // SRC_PARSER_NODEWAYRELATIONPARSER_H_
