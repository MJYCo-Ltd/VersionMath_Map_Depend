INCLUDE_DIRECTORIES( ${ILMBASE_INCLUDE_DIR}/OpenEXR )
INCLUDE_DIRECTORIES( ${OPENEXR_INCLUDE_DIR}/OpenEXR )

SET(TARGET_SRC ReaderWriterEXR.cpp )

SET(TARGET_LIBRARIES_VARS ${OPENEXR_LIBRARIES_VARS} ${ILMBASE_LIBRARIES_VARS} ZLIB_LIBRARIES)

IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    REMOVE_CXX_FLAG(-Wshadow)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
ENDIF()

#### end var setup  ###
SETUP_PLUGIN(exr)
