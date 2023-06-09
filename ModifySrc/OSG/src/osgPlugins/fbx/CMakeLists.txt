INCLUDE_DIRECTORIES(${FBX_INCLUDE_DIR})

SET(TARGET_SRC
    fbxRAnimation.cpp
    fbxRCamera.cpp
    fbxRLight.cpp
    fbxRMesh.cpp
    fbxRNode.cpp
    ReaderWriterFBX.cpp
    WriterCompareTriangle.cpp
    WriterNodeVisitor.cpp
    fbxMaterialToOsgStateSet.cpp
)

SET(TARGET_H
    fbxMaterialToOsgStateSet.h
    fbxReader.h
    ReaderWriterFBX.h
    WriterCompareTriangle.h
    WriterNodeVisitor.h
)

IF(CMAKE_COMPILER_IS_GNUCXX)

    SET(CMAKE_CXX_FLAGS "-w")

ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    REMOVE_CXX_FLAG(-pedantic)
    REMOVE_CXX_FLAG(-Wall)
    REMOVE_CXX_FLAG(-Wunused)
    REMOVE_CXX_FLAG(-Wextra)

ENDIF()

SET(TARGET_LIBRARIES_VARS FBX_LIBRARY)
IF(WIN32)
    OPTION(FBX_SHARED OFF)
    IF(FBX_SHARED)
        ADD_DEFINITIONS(-DFBXSDK_SHARED)
    ENDIF(FBX_SHARED)

    SET(TARGET_EXTERNAL_LIBRARIES wininet)
ENDIF(WIN32)

IF(APPLE)
    FIND_LIBRARY(SYS_CONFIG_LIBRARY SystemConfiguration )
    MARK_AS_ADVANCED(SYS_CONFIG_LIBRARY)
    SET(TARGET_EXTERNAL_LIBRARIES iconv ${SYS_CONFIG_LIBRARY})
ENDIF(APPLE)

IF(FBX_XML2_LIBRARY)
    SET(TARGET_LIBRARIES_VARS ${TARGET_LIBRARIES_VARS} FBX_XML2_LIBRARY)
ENDIF(FBX_XML2_LIBRARY)
IF(FBX_ZLIB_LIBRARY)
    SET(TARGET_LIBRARIES_VARS ${TARGET_LIBRARIES_VARS} FBX_ZLIB_LIBRARY)
ENDIF(FBX_ZLIB_LIBRARY)

SET(TARGET_ADDED_LIBRARIES osgAnimation)
#### end var setup  ###
SETUP_PLUGIN(fbx)
