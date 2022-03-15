// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include "util/console/Console.h"
#include <getopt.h>
#include <iostream>
#include "global/Constants.h"

using global::DEFAULT_ELE_TAG;
using util::console::CommandLineArgsAdd;
using util::console::CommandLineArgsCorrect;
using util::console::ProgressBar;

// ____________________________________________________________________________
ProgressBar::ProgressBar(const uint64_t& total) {
  _total = static_cast<double>(total);
  update(0, true);
}

// ____________________________________________________________________________
void ProgressBar::update(const uint64_t& currentProgress,
                         const bool& done) const {
  if ((currentProgress % 10000 == 0) || done) {
    std::cout << "[";
    const u_int16_t progress = static_cast<double>(_barWidth *
                                                   (currentProgress / _total));

    for (int i = 0; i < _barWidth; ++i) {
      if (i < progress) {
        std::cout << "="; } else if ( i == progress ) {
          std::cout << ">"; } else {
            std::cout << " "; }
    }
    std::cout << "] " << static_cast<int>(currentProgress / _total * 100.0);
    std::cout << "%\r";
    std::cout.flush();
  }
}

// ____________________________________________________________________________
void ProgressBar::done() const {
  update(_total, true);
  std::cout << "\n" << std::endl;
}

// ____________________________________________________________________________
void util::console::printUsageAndExitAdd() {
  std::cerr << "Usage: ./osmelevation [option] <NASADEM files directory> ";
  std::cerr << "<OSM input file> ";
  std::cerr << "<OSM output file>" << std::endl;
  std::cerr << "Available option:" << std::endl;
  std::cerr << "--tag <tag key>: The elevation tag used to add the ";
  std::cerr << "elevation to each node." << std::endl;
  std::cerr << "(default: 'ele')" << std::endl;
  exit(1);
}

// ____________________________________________________________________________
void util::console::printUsageAndExitCorrect() {
  std::cerr << "Usage: ./correctosmelevation [option] <OSM input file> ";
  std::cerr << "<OSM output file> " << std::endl;
  std::cerr << "Available option:" << std::endl;
  std::cerr << "--tag <tag key>: The elevation tag on which the corrections ";
  std::cerr << "are performed." << std::endl;
  std::cerr << "(default: 'ele')" << std::endl;
  exit(1);
}

// ____________________________________________________________________________
CommandLineArgsAdd util::console::parseCommandLineArgumentsAdd(int argc,
                                                               char** argv) {
  struct option options[] = {
    {"tag", 1, NULL, 't'},
    {NULL, 0, NULL, 0}
  };
  optind = 1;

  // Default value
  std::string elevationTag = DEFAULT_ELE_TAG;

  while (true) {
    char t = getopt_long(argc, argv, "t:", options, NULL);
    if (t == -1) { break; }
    switch (t) {
      case 't':
        elevationTag = optarg;
        break;
      case '?':
      default:
        util::console::printUsageAndExitAdd();
    }
  }

  if (optind + 3 != argc) {
    util::console::printUsageAndExitAdd();
  }
  CommandLineArgsAdd args;
  args.nasademDir = argv[optind];
  args.inputFile = argv[optind + 1];
  args.outputFile = argv[optind + 2];
  args.elevationTag = elevationTag;

  return args;
}

// ____________________________________________________________________________
CommandLineArgsCorrect util::console::parseCommandLineArgumentsCorrect(
    int argc, char** argv) {
  struct option options[] = {
    {"tag", 1, NULL, 't'},
    {NULL, 0, NULL, 0}
  };
  optind = 1;

  // Default value
  std::string elevationTag = DEFAULT_ELE_TAG;

  while (true) {
    char t = getopt_long(argc, argv, "t:", options, NULL);
    if (t == -1) { break; }
    switch (t) {
      case 't':
        elevationTag = optarg;
        break;
      case '?':
      default:
        util::console::printUsageAndExitCorrect();
    }
  }
  if (optind + 2 != argc) {
    util::console::printUsageAndExitCorrect();
  }
  CommandLineArgsCorrect args;
  args.inputFile = argv[optind];
  args.outputFile = argv[optind + 1];
  args.elevationTag = elevationTag;

  return args;
}
