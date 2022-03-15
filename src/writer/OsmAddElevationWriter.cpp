// Copyright 2022, Urs Spiegelhalter
// Author: Urs Spiegelhalter <urs.sp99@gmail.com>.

#include <string>
#include <utility>
#include <ctime>
#include <osmium/io/any_input.hpp>
#include <osmium/io/any_output.hpp>
#include <osmium/visitor.hpp>
#include <osmium/util/progress_bar.hpp>
#include "util/index/ElevationIndex.h"
#include "writer/AddElevationTags.h"
#include "writer/OsmAddElevationWriter.h"

using util::index::ElevationIndex;
using writer::AddElevationTags;
using writer::OsmAddElevationWriter;

// ____________________________________________________________________________
OsmAddElevationWriter::OsmAddElevationWriter(
    const std::string& inFile,
    const std::string& outFile,
    ElevationIndex* elevationIndex,
    const std::string& elevationTag,
    const bool overwrite) :
    _inFile(inFile),
    _outFile(outFile),
    _elevationIndex(elevationIndex),
    _elevationTag(elevationTag),
    _overwrite(overwrite) {
}

// ____________________________________________________________________________
void OsmAddElevationWriter::write() {
  time_t start, end;
  start = time(&start);
  std::cout << "\nAdding the elevation tags to the output file ";
  std::cout << _outFile << std::endl;
  // Initialize Reader
  osmium::io::Reader reader{_inFile};

  osmium::io::Header header = reader.header();
  header.set("generator", "osmium_add_elevation_tags");

  osmium::io::Writer writer {
    _outFile, header
    };
  // Initialize progress bar, enable it only if STDERR is a TTY.
  osmium::ProgressBar progress{reader.file_size(), osmium::isatty(2)};
  // Read in buffers with OSM objects until there are no more.
  uint64_t count = 0;
  while (osmium::memory::Buffer input_buffer = reader.read()) {
    ++count;
    // No need to update for every single entity.
    if (count % 10000 == 0) {
      progress.update(reader.offset());
    }
    // Create an empty buffer with the same size as the input buffer.
    // We'll copy the changed data into output buffer, the changes
    // are small, so the output buffer needs to be about the same size.
    // In case it has to be bigger, we allow it to grow automatically
    // by adding the auto_grow::yes parameter.
    osmium::memory::Buffer output_buffer{
      input_buffer.committed(), osmium::memory::Buffer::auto_grow::yes
      };
    // Construct a handler as defined above and feed the input buffer
    // to it.
    AddElevationTags handler{output_buffer, _elevationIndex,
                             _elevationTag, _overwrite};
    osmium::apply(input_buffer, handler);

    // Write out the contents of the output buffer.
    writer(std::move(output_buffer));
  }
  // Progress bar is done.
  progress.done();
  writer.close();
  reader.close();

  end = time(&end);
  double timeDiff = difftime(end, start);
  std::cout << "Done, writing the output file took ";
  std::cout << timeDiff;
  std::cout << " seconds." << "\n" << std::endl;
}
