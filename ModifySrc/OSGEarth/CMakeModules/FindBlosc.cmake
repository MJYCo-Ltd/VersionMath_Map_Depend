# Locate BLOSC.
# This module defines
# BLOSC_LIBRARY
# BLOSC_FOUND, if false, do not try to link to blosc
# BLOSC_INCLUDE_DIR, where to find the headers

SET(BLOSC_DIR "" CACHE PATH "Root directory of BLOSC distribution")

FIND_PATH(BLOSC_INCLUDE_DIR blosc.h
  PATHS
    ${BLOSC_DIR}
    $ENV{BLOSC_DIR}
  PATH_SUFFIXES include
)

FIND_LIBRARY(BLOSC_LIBRARY
  NAMES blosc
  PATHS  
    ${BLOSC_DIR}/lib
    $ENV{BLOSC_DIR}
  PATH_SUFFIXES lib64 lib
)

SET(BLOSC_FOUND "NO")
IF(BLOSC_LIBRARY AND BLOSC_INCLUDE_DIR)
  SET(BLOSC_FOUND "YES")
ENDIF(BLOSC_LIBRARY AND BLOSC_INCLUDE_DIR)