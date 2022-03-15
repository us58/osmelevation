add_library(writer
        AddElevationTags.h AddElevationTags.cpp
        OsmAddElevationWriter.h OsmAddElevationWriter.cpp)

target_link_libraries(writer)