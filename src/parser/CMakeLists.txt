add_library(parser
        NodeParser.h NodeParser.cpp
        WayParser.h WayParser.cpp
        RelationParser.h RelationParser.cpp
        NodeWayRelationParser.h NodeWayRelationParser.cpp)

add_library(osmhandler OsmHandler.h OsmHandler.cpp)

target_link_libraries(parser osmhandler)