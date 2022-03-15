// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <ctime>
#include "correctosmelevation/osm/CorrectElevation.h"
#include "util/console/Console.h"

using correctosmelevation::osm::CorrectElevation;
using util::console::CommandLineArgsCorrect;
using util::console::parseCommandLineArgumentsCorrect;

bool validArguments(CommandLineArgsCorrect args);

// _____________________________________________________________________________
int main(int argc, char** argv) {
  try {
    time_t start, end;
    start = time(&start);
    const CommandLineArgsCorrect args = parseCommandLineArgumentsCorrect(argc,
                                                                         argv);

    if (!validArguments(args)) {
      return 0;
    }

    const uint64_t availableMemoryInGB = (sysconf(_SC_PHYS_PAGES) *
                                          sysconf(_SC_PAGESIZE) /
                                         1073741824) - 5;

    const auto routeWaysPerRange = availableMemoryInGB * 500000;

    const auto routeRelationsPerRange = availableMemoryInGB * 50000;

    CorrectElevation correctElevation(args.inputFile, args.outputFile,
                                      args.elevationTag, routeWaysPerRange,
                                      routeRelationsPerRange);
    correctElevation.initialize();

    correctElevation.correctRoutes();

    correctElevation.writeOutputOSM();

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
uint16_t ava(const uint64_t& nodeCount) {
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
bool validArguments(CommandLineArgsCorrect args) {
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
