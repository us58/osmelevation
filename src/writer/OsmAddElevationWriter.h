// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#ifndef SRC_WRITER_OSMADDELEVATIONWRITER_H_
#define SRC_WRITER_OSMADDELEVATIONWRITER_H_

#include <string>
#include "util/index/ElevationIndex.h"

namespace writer {

using util::index::ElevationIndex;

class OsmAddElevationWriter {
 public:
  OsmAddElevationWriter(const std::string& inFile,
                        const std::string& outFile,
                        ElevationIndex* elevationIndex,
                        const std::string& elevationTag,
                        const bool overwrite);

  void write();

 private:
  const std::string& _inFile;

  const std::string& _outFile;

  ElevationIndex* _elevationIndex;

  const std::string& _elevationTag;

  const bool _overwrite;
};

}  // namespace writer

#endif  // SRC_WRITER_OSMADDELEVATIONWRITER_H_
