OPTION(CURL_IS_STATIC "on if curl is a static lib " OFF)
# MARK_AS_ADVANCED(CURL_IS_STATIC)

IF(WIN32)
    SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:MSVCRT")
ENDIF()

IF(ZLIB_FOUND)
    ADD_DEFINITIONS(-DUSE_ZLIB)
    INCLUDE_DIRECTORIES( ${CURL_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIR})
ELSE()
    INCLUDE_DIRECTORIES( ${CURL_INCLUDE_DIRS} )
ENDIF()


SET(TARGET_SRC
    ReaderWriterCURL.cpp
)

SET(TARGET_H
    ReaderWriterCURL.h
)

IF(ZLIB_FOUND)   
    SET(TARGET_LIBRARIES_VARS
        CURL_LIBRARIES
        ZLIB_LIBRARIES)
ELSE()
    SET(TARGET_LIBRARIES_VARS
        CURL_LIBRARIES)
ENDIF()

message(STATUS "tttt ${CURL_LIBRARIES}")
IF(WIN32 OR MINGW)
    IF(CURL_IS_STATIC)
        ADD_DEFINITIONS(-DCURL_STATICLIB)
        SET(TARGET_EXTERNAL_LIBRARIES ${TARGET_EXTERNAL_LIBRARIES} ws2_32 winmm wldap32)
        message(STATUS "111111 ${TARGET_LIBRARIES_VARS}")
    ENDIF()
ENDIF()

#### end var setup  ###
SETUP_PLUGIN(curl)
