// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <osmium/util/progress_bar.hpp>
#include <osmium/visitor.hpp>
#include <osmium/io/any_input.hpp>
#include "parser/NodeWayRelationParser.h"

using parser::NodeWayRelationParser;
using parser::OsmHandler;

// ____________________________________________________________________________
NodeWayRelationParser::NodeWayRelationParser(const std::string& osmFile,
                                             OsmHandler* handler) :
                                             _osmFile(osmFile),
                                             _handler(handler) {}

// ____________________________________________________________________________
void NodeWayRelationParser::parse() {
  osmium::io::File input_file{_osmFile};
  osmium::io::Reader reader{input_file};

  // Initialize progress bar, enable it only if STDERR is a TTY.
  osmium::ProgressBar progress{reader.file_size(), osmium::isatty(2)};

  uint64_t count = 0;
  // OSM data comes in buffers, read until there are no more.
  while (osmium::memory::Buffer buffer = reader.read()) {
    ++count;
    // No need to update for every single entity.
    if (count % 10000 == 0) {
      progress.update(reader.offset());
    }
    osmium::apply(buffer, *_handler);
  }
  // Progress bar is done.
  progress.done();
  reader.close();
}
