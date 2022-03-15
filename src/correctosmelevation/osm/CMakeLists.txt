add_library(correctelevationosm
        OsmNodesHandler.h OsmNodesHandler.cpp
        OsmWaysHandler.h OsmWaysHandler.cpp
        OsmRelationsManager.h OsmRelationsManager.cpp
        CorrectElevation.h CorrectElevation.cpp
        OsmRoutesRange.h OsmRoutesRange.cpp
        RoutesFromRelations.h RoutesFromRelations.cpp
        RoutesFromWays.h RoutesFromWays.cpp
        ProcessRouteRelation.h ProcessRouteRelation.cpp
        FindTunnelsAndBridges.h FindTunnelsAndBridges.cpp)

target_link_libraries(correctelevationosm)