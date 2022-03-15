add_executable(ElevationIndexSparseTest ElevationIndexSparseTest.cpp)
add_test(NAME ElevationIndexSparseTest COMMAND ElevationIndexSparseTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(ElevationIndexSparseTest util gtest_main ${TBB_LIBRARIES})

add_executable(ElevationIndexDenseTest ElevationIndexDenseTest.cpp)
add_test(NAME ElevationIndexDenseTest COMMAND ElevationIndexDenseTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(ElevationIndexDenseTest util gtest_main)

add_executable(AverageElevationIndexSparseTest AverageElevationIndexSparseTest.cpp)
add_test(NAME AverageElevationIndexSparseTest COMMAND AverageElevationIndexSparseTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(AverageElevationIndexSparseTest util gtest_main ${TBB_LIBRARIES})

add_executable(NasademFileNameTest NasademFileNameTest.cpp)
add_test(NAME NasademFileNameTest COMMAND NasademFileNameTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(NasademFileNameTest osmelevationelevation gtest_main)

add_executable(NasademFileTest NasademFileTest.cpp)
add_test(NAME NasademFileTest COMMAND NasademFileTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(NasademFileTest osmelevationelevation ${LIBZIP_LIBRARY} gtest_main)

add_executable(GeoElevationTest GeoElevationTest.cpp)
add_test(NAME GeoElevationTest COMMAND GeoElevationTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(GeoElevationTest osmelevationelevation ${LIBZIP_LIBRARY} gtest_main)

add_executable(GeoTilesTest GeoTilesTest.cpp)
add_test(NAME GeoTilesTest COMMAND GeoTilesTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(GeoTilesTest osmelevationosm util gtest_main)

add_executable(ElevationsInTileTest ElevationsInTileTest.cpp)
add_test(NAME ElevationsInTileTest COMMAND ElevationsInTileTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(ElevationsInTileTest osmelevationosm osmelevationelevation parser util ${OSMIUM_LIBRARIES} ${LIBZIP_LIBRARY} ${TBB_LIBRARIES} gtest_main)

add_executable(GraphTest GraphTest.cpp)
add_test(NAME GraphTest COMMAND GraphTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(GraphTest util gtest_main)

add_executable(CorrectRiverTest CorrectRiverTest.cpp)
add_test(NAME CorrectRiverTest COMMAND CorrectRiverTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(CorrectRiverTest correctelevationcorrect util ${TBB_LIBRARIES} gtest_main)

add_executable(CorrectTunnelOrBridgeTest CorrectTunnelOrBridgeTest.cpp)
add_test(NAME CorrectTunnelOrBridgeTest COMMAND CorrectTunnelOrBridgeTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(CorrectTunnelOrBridgeTest correctelevationcorrect util ${TBB_LIBRARIES} gtest_main)

add_executable(SmoothRouteTest SmoothRouteTest.cpp)
add_test(NAME SmoothRouteTest COMMAND SmoothRouteTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(SmoothRouteTest correctelevationcorrect utsalgorithms util ${TBB_LIBRARIES} gtest_main)

add_executable(RoutesFromRelationsTest RoutesFromRelationsTest.cpp)
add_test(NAME RoutesFromRelationsTest COMMAND RoutesFromRelationsTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(RoutesFromRelationsTest correctelevationosm parser util ${OSMIUM_LIBRARIES} ${EXPAT_LIBRARIES} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES} gtest_main)

add_executable(RoutesFromWaysTest RoutesFromWaysTest.cpp)
add_test(NAME RoutesFromWaysTest COMMAND RoutesFromWaysTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(RoutesFromWaysTest correctelevationosm parser util ${OSMIUM_LIBRARIES} ${EXPAT_LIBRARIES} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES} gtest_main)

add_executable(CorrectElevationTest CorrectElevationTest.cpp)
add_test(NAME CorrectElevationTest COMMAND CorrectElevationTest WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(CorrectElevationTest
        correctelevationosm
        correctelevationcorrect
        utsalgorithms
        parser
        osmhandler
        writer
        util
        ${LIBZIP_LIBRARY}
        ${OSMIUM_LIBRARIES}
        ${EXPAT_LIBRARIES}
        ${BZIP2_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${TBB_LIBRARIES}
        -lpthread
        gtest_main)

add_executable(UtilTests UtilTests.cpp)
add_test(NAME UtilTests COMMAND UtilTests WORKING_DIRECTORY "${DIRECTORY_WITH_TEST_DATA}")
target_link_libraries(UtilTests util parser ${OSMIUM_LIBRARIES} ${EXPAT_LIBRARIES} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES} gtest_main)
