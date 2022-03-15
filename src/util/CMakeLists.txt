file(GLOB_RECURSE util_SRC *.cpp)

add_library(util ${util_SRC})

target_link_libraries(util)