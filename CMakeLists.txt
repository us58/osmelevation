cmake_minimum_required(VERSION 3.12)
project(OsmElevation)

# C++ Versions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check compiler versions:
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Check version. If empty, warn. If too old, error out:
  if("${CMAKE_CXX_COMPILER_VERSION}" STREQUAL "")
    message(WARNING "GCC Compiler version is unknown, proceed at your own risk!")
  elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8)
    message(FATAL_ERROR "GCC compiler version must be at least 4.8!")
  endif()
endif()

###############################################################################
##### CPPLINT #####
###############################################################################
set(CPPLINT "${CMAKE_SOURCE_DIR}/cpplint.py")
include(cmake/cpplint.cmake)
cpplint_add_subdirectory("src")
set(CPPLINT_PROJECT_ROOT "src")

###############################################################################
##### Essential settings #####
###############################################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(DIRECTORY_WITH_TEST_DATA "${CMAKE_SOURCE_DIR}/test/test_files/")

add_compile_options(-Ofast -Wall -Wno-format-extra-args -Wextra -Wformat-nonliteral -Wformat-security -Wformat=2 -Wextra -Wno-implicit-fallthrough -pedantic)
add_compile_options(-march=native -flto)

################################
# GTEST
################################
add_subdirectory(third_party/googletest/googletest)
include_directories(third_party/googletest/googletest/include)

################################
# UTSAlgorithms
################################
add_library(utsalgorithms
        third_party/utsAlgorithms/ema.h third_party/utsAlgorithms/ema.h
        third_party/utsAlgorithms/rolling.h third_party/utsAlgorithms/rolling.c
        third_party/utsAlgorithms/sma.h third_party/utsAlgorithms/sma.c)
target_link_libraries(utsalgorithms)
include_directories(third_party/utsAlgorithms)

################################
# BOOST
################################
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

################################
# EXPAT
################################
find_package(EXPAT REQUIRED)

################################
# BZIP2
################################
find_package(BZip2 REQUIRED)

################################
# ZLIB
################################
find_package(ZLIB REQUIRED)

################################
# PROTOZERO
################################
find_package(Protozero REQUIRED)

################################
# LIBOSMIUM
################################
find_package(Osmium REQUIRED)
include_directories(SYSTEM ${OSMIUM_INCLUDE_DIRS})

################################
# LIBZIP
################################
find_package(LibZip REQUIRED)
include_directories(SYSTEM ${LIBZIP_INCLUDE_DIR})

###############################################################################
##### Actual project configuration #####
###############################################################################

include_directories(src)

add_subdirectory(src/util)
add_subdirectory(src/writer)
add_subdirectory(src/parser)
add_subdirectory(src/osmelevation/elevation)
add_subdirectory(src/osmelevation/osm)
add_subdirectory(src/correctosmelevation/osm)
add_subdirectory(src/correctosmelevation/correct)

add_executable(osmelevation src/OsmElevationMain.cpp)

add_executable(correctosmelevation src/CorrectOsmElevationMain.cpp)

target_link_libraries(osmelevation
        osmelevationosm
        osmelevationelevation
        parser
        writer
        util
        ${LIBZIP_LIBRARY}
        ${OSMIUM_LIBRARIES}
        ${EXPAT_LIBRARIES}
        ${BZIP2_LIBRARIES}
        ${ZLIB_LIBRARIES}
        -lpthread)

target_link_libraries(correctosmelevation
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
        -lpthread)

enable_testing()
add_subdirectory(test)