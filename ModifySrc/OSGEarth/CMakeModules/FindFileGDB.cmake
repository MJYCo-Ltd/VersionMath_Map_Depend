# Locate ESRI FileGDB API libraries.
# This module defines
# FILEGDB_LIBRARY
# FILEGDB_LIBRARY_DEBUG
# FILEGDB_FOUND, if false, do not try to link to ESRI FileGDB API
# FILEGDB_INCLUDE_DIR, where to find the headers

SET(FILEGDB_DIR "" CACHE PATH "Set to ESRI FileGDB install location")

FIND_PATH(FILEGDB_INCLUDE_DIR FileGDBAPI.h
  PATHS
    ${FILEGDB_DIR}/include
    $ENV{FILEGDB_DIR}/include
    $ENV{FILEGDB_DIR}
    /devel/filegdb/include
)

FIND_LIBRARY(FILEGDB_LIBRARY FileGDBAPI
    NAMES FileGDBAPI
    PATHS ${FILEGDB_DIR}
    PATH_SUFFIXES /lib64/ /lib/
)

FIND_LIBRARY(FILEGDB_LIBRARY_DEBUG FileGDBAPID
    NAMES FileGDBAPID
    PATHS ${FILEGDB_DIR}
    PATH_SUFFIXES /lib64/ /lib/
)

SET(FILEGDB_FOUND "NO")
IF(FILEGDB_LIBRARY AND FILEGDB_INCLUDE_DIR)
    SET(FILEGDB_FOUND "YES")
ENDIF()


