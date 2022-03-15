// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <ctime>
#include "osmelevation/osm/GeoPartition.h"
#include "osmelevation/osm/GeoBoundaries.h"
#include "parser/NodeWayRelationParser.h"
#include "util/console/Console.h"
#include "util/osm/OsmStats.h"
#include "util/osm/GetOsmStats.h"
#include "util/index/ElevationIndex.h"
#include "util/index/ElevationIndexDense.h"
#include "util/index/ElevationIndexSparse.h"
#include "writer/OsmAddElevationWriter.h"

using osmelevation::osm::GeoBoundaries;
using osmelevation::osm::GeoPartition;
using parser::NodeWayRelationParser;
using util::console::parseCommandLineArgumentsAdd;
using util::console::CommandLineArgsAdd;
using util::osm::OsmStats;
using util::osm::GetOsmStats;
using util::index::ElevationIndex;
using util::index::ElevationIndexDense;
using util::index::ElevationIndexSparse;
using writer::OsmAddElevationWriter;

void run(const CommandLineArgsAdd& args);
OsmStats getOsmStats(const std::string& osmFile);
bool validArguments(CommandLineArgsAdd args);
uint16_t nasademFilesInMemory(const uint64_t& nodeCount);
uint16_t getBoundarySize(const uint16_t maxInMemory);

// _____________________________________________________________________________
int main(int argc, char** argv) {
  try {
    time_t start, end;
    start = time(&start);
    const CommandLineArgsAdd args = parseCommandLineArgumentsAdd(argc, argv);
    if (!validArguments(args)) {
      return 0;
    }
    run(args);

    end = time(&end);
    double timeDiff = difftime(end, start);
    std::cout << "Program completed, took ";
    std::cout << timeDiff;
    std::cout << " seconds total." << "\n" << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

// _____________________________________________________________________________
void run(const CommandLineArgsAdd& args) {
  // Initialize.
  OsmStats osmStats = getOsmStats(args.inputFile);
  uint16_t maxInMemory = nasademFilesInMemory(osmStats.nodeCount);

  // Depending on the number of total nodes, choose
  // dense or sparse array for elevation storage.
  std::unique_ptr<ElevationIndex> elevationIndex;
  if (osmStats.nodeCount > 1000000000) {
    elevationIndex =
      std::make_unique<ElevationIndexDense>(osmStats.nodeCount,
                                            osmStats.max);
  } else {
    elevationIndex =
      std::make_unique<ElevationIndexSparse>(osmStats.nodeCount,
                                             osmStats.max);
  }

  // Get the boundaries for all geo partitions.
  GeoBoundaries geoBoundaries(getBoundarySize(maxInMemory), osmStats);
  const auto boundaries = geoBoundaries.buildBoundaries();

  // Work off all geographic partitions and
  // collect the elevation for each node.
  for (const auto& boundary : boundaries) {
    GeoPartition geoPartition(*elevationIndex, osmStats,
                              args.inputFile, args.nasademDir, boundary);
    geoPartition.elevationsInPartition();
  }

  // Sort the index if sparse was used.
  elevationIndex->process();

  // Write the result to the specified output osm file.
  OsmAddElevationWriter writer(args.inputFile, args.outputFile,
                               elevationIndex.get(), args.elevationTag, false);
  writer.write();
}

// _____________________________________________________________________________
bool validArguments(CommandLineArgsAdd args) {
  bool valid = true;
  // Check if the input osm file exists.
  if (!std::filesystem::exists(args.inputFile)) {
    valid = false;
    std::cerr << "Invalid input file: File does not exist." << std::endl;
  }
  // Check that a file with the provided output name does not already exist.
  if (std::filesystem::exists(args.outputFile)) {
    valid = false;
    std::cerr << "Invalid output file: File does already exist." << std::endl;
  }
  return valid;
}

// _____________________________________________________________________________
uint16_t nasademFilesInMemory(const uint64_t& nodeCount) {
  const uint64_t availableMem = sysconf(_SC_PHYS_PAGES) *
                                sysconf(_SC_PAGESIZE);

  // Dense node elevation index needs 16GB memory for the 9 billions node
  // (14 bits per node).
  // For the sparse node elevation index, one nodes needs 64 + 16 = 80 bits.
  // As soon as the total node count of the input osm file exceeds
  // 1 billion, the dense index is used, otherwise the sparse.
  const uint64_t indexMem = (nodeCount < 1600000000) ? nodeCount * 10
                                                     : 16000000000;

  // The osmium library which is used for parsing, needs around 5GB memory for
  // fast parsing.
  const uint64_t parsingMem = 5000000000;

  // The remaining available memory that can be used to store as many
  // NASADEM files in memory at the same time as possible.
  const int64_t nasademMem = availableMem - indexMem - parsingMem;

  // One NASADEM file needs 26MB.
  return (nasademMem > 1) ? nasademMem / 27262976 : 1;
}

// _____________________________________________________________________________
OsmStats getOsmStats(const std::string& osmFile) {
  GetOsmStats handler;
  NodeWayRelationParser statsParser(osmFile, &handler);

  time_t start, end;
  start = time(&start);
  std::cout << "Collecting statistics about the input file ";
  std::cout << osmFile << std::endl;

  statsParser.parse();

  end = time(&end);
  double timeDiff = difftime(end, start);
  std::cout << "Done, took " << timeDiff;
  std::cout << " seconds." << "\n" << std::endl;

  OsmStats osmStats = handler.getOsmStats();

  return osmStats;
}

// _____________________________________________________________________________
uint16_t getBoundarySize(const uint16_t maxInMemory) {
  int16_t tileSizeReduced = floor(sqrt(maxInMemory)) - 2;
  return static_cast<uint16_t>((tileSizeReduced > 1) ? tileSizeReduced : 1);
}
