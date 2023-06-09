# Locate BASISU.
# This module defines
# BASISU_TRANSCODER_LIBRARY
# BASISU_FOUND, if false, do not try to link to geos
# BASISU_INCLUDE_DIR, where to find the headers

#MESSAGE("Finding basis")

SET(BASISU_DIR "" CACHE PATH "Root directory of BASISU distribution")

FIND_PATH(BASISU_INCLUDE_DIR basisu/transcoder/basisu.h
  PATHS
    ${BASISU_DIR}
    $ENV{BASISU_DIR}
  PATH_SUFFIXES include
)

FIND_LIBRARY(BASISU_TRANSCODER_LIBRARY
  NAMES basisu_transcoder
  PATHS  
    ${BASISU_DIR}/lib
    $ENV{BASISU_DIR}
  PATH_SUFFIXES lib64 lib
)

SET(BASISU_FOUND "NO")
IF(BASISU_TRANSCODER_LIBRARY AND BASISU_INCLUDE_DIR)
  SET(BASISU_FOUND "YES")
ENDIF(BASISU_TRANSCODER_LIBRARY AND BASISU_INCLUDE_DIR)

#MESSAGE("Basis found ${BASISU_FOUND}")
#MESSAGE("Basis include ${BASISU_INCLUDE_DIR}")
#MESSAGE("Basis transcoder ${BASISU_TRANSCODER_LIBRARY}")
