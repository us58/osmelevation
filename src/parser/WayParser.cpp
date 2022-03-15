// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <osmium/io/any_input.hpp>
#include <osmium/visitor.hpp>
#include "util/console/Console.h"
#include "util/osm/OsmStats.h"
#include "parser/OsmHandler.h"
#include "parser/WayParser.h"

using util::console::ProgressBar;
using util::osm::OsmStats;
using parser::OsmHandler;
using parser::WayParser;

// ____________________________________________________________________________
WayParser::WayParser(const std::string& osmFile,
                     OsmHandler* handler,
                     const OsmStats& osmStats) :
                     _osmFile(osmFile),
                     _handler(handler),
                     _osmStats(osmStats) {}

// ____________________________________________________________________________
void WayParser::parse() {
  osmium::io::File input_file{_osmFile};
  osmium::io::Reader reader{input_file, osmium::osm_entity_bits::way};

  // Initialize progress bar.
  ProgressBar progressBar(_osmStats.wayCount);

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
void WayParser::parseNoProgressBar() {
  osmium::io::File input_file{_osmFile};
  osmium::io::Reader reader{input_file, osmium::osm_entity_bits::way};

  // OSM data comes in buffers, read until there are no more.
  while (osmium::memory::Buffer buffer = reader.read()) {
    // Update progress bar for each buffer.
    osmium::apply(buffer, *_handler);
  }
  reader.close();
}
