INCLUDE_DIRECTORIES( ${GDAL_INCLUDE_DIR} )

SET(TARGET_SRC
    ReaderWriterGDAL.cpp
    DataSetLayer.cpp
)

SET(TARGET_H
    DataSetLayer.h
)

SET(TARGET_LIBRARIES_VARS GDAL_LIBRARY )
SET(TARGET_ADDED_LIBRARIES osgTerrain )

IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    REMOVE_CXX_FLAG(-Wunused-result)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")
ENDIF()

#### end var setup  ###
SETUP_PLUGIN(gdal)
