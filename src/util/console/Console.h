// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_UTIL_CONSOLE_CONSOLE_H_
#define SRC_UTIL_CONSOLE_CONSOLE_H_

#include <cstdint>
#include <string>

namespace util {
namespace console {

struct CommandLineArgsAdd {
 public:
  std::string nasademDir;
  std::string inputFile;
  std::string outputFile;
  std::string elevationTag;
};

struct CommandLineArgsCorrect {
 public:
  std::string inputFile;
  std::string outputFile;
  std::string elevationTag;
};

/*
* Given an amount of total entities that have to be worked off,
* show a progress bar.
*/
class ProgressBar {
 public:
  // Construct with the total number of entities.
  explicit ProgressBar(const uint64_t& total);

  // Update the progress bar according to the current number of
  // already worked off entities.
  void update(const uint64_t& currentProgress, const bool& done) const;

  // Completely fill the progress bar and end the line.
  void done() const;

 private:
  const double _barWidth = 70.0;
  double _total;
};

void printUsageAndExitAdd();
void printUsageAndExitCorrect();

CommandLineArgsAdd parseCommandLineArgumentsAdd(int argc, char** argv);
CommandLineArgsCorrect parseCommandLineArgumentsCorrect(int argc, char** argv);

}  // namespace console
}  // namespace util

#endif  // SRC_UTIL_CONSOLE_CONSOLE_H_
