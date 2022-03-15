find_path(LIBZIP_INCLUDE_DIR
    zip.h
    /usr/local/include
    /usr/include
)

find_path(LIBZIP_INCLUDE_DIR_CONF
    zipconf.h
    /usr/local/lib/libzip/include
    /usr/lib/libzip/include
    /usr/local/include
    /usr/include
)

find_library(LIBZIP_LIBRARY
    zip
    /usr/local/lib
    /usr/lib
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    LIBZIP DEFAULT_MSG
    LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR LIBZIP_INCLUDE_DIR_CONF
)

IF (LIBZIP_FOUND)
  MESSAGE(STATUS "Found libzip: ${LIBZIP_LIBRARY}")
ELSE (LIPZIP_FOUND)
  MESSAGE(FATAL_ERROR "Could not find libzip")
ENDIF (LIBZIP_FOUND)