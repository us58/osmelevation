add_library(osmelevationosm
        GeoBoundaries.h GeoBoundaries.cpp
        GeoPartition.h GeoPartition.cpp
        OsmNodesHandler.h OsmNodesHandler.cpp)

target_link_libraries(osmelevationosm)