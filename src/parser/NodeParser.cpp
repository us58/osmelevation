// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <osmium/visitor.hpp>
#include <osmium/io/any_input.hpp>
#include "util/console/Console.h"
#include "util/osm/OsmStats.h"
#include "parser/OsmHandler.h"
#include "parser/NodeParser.h"

using util::console::ProgressBar;
using util::osm::OsmStats;
using parser::OsmHandler;
using parser::NodeParser;

// ____________________________________________________________________________
NodeParser::NodeParser(const std::string& osmFile,
                       OsmHandler* handler,
                       const OsmStats& osmStats) :
                       _osmFile(osmFile),
                       _handler(handler),
                       _osmStats(osmStats) {}

// ____________________________________________________________________________
void NodeParser::parse() {
  osmium::io::File input_file{_osmFile};
  osmium::io::Reader reader{input_file, osmium::osm_entity_bits::node};

  // Initialize progress bar.
  ProgressBar progressBar(_osmStats.nodeCount);

  // OSM data comes in buffers, read until there are no more.
  while (osmium::memory::Buffer buffer = reader.read()) {
    // Update progress bar for each buffer.
    progressBar.update(_handler->getCount(), false);
    osmium::apply(buffer, *_handler);
  }
  // Progress bar is done.
  progressBar.done();
  reader.close();
}

// ____________________________________________________________________________
void NodeParser::parseNoProgressBar() {
  osmium::io::File input_file{_osmFile};
  osmium::io::Reader reader{input_file, osmium::osm_entity_bits::node};

  // OSM data comes in buffers, read until there are no more.
  while (osmium::memory::Buffer buffer = reader.read()) {
    // Update progress bar for each buffer.
    osmium::apply(buffer, *_handler);
  }
  reader.close();
}
