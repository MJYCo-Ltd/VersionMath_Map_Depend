INCLUDE_DIRECTORIES( ${FFMPEG_INCLUDE_DIRS} )
LINK_DIRECTORIES(${FFMPEG_LIBRARY_DIRS})

IF(FFMPEG_LIBSWSCALE_FOUND)

    INCLUDE_DIRECTORIES( ${FFMPEG_LIBSWSCALE_INCLUDE_DIRS} ${FFMPEG_LIBSWSCALE_INCLUDE_DIRS}/libswscale )

    ADD_DEFINITIONS(-DUSE_SWSCALE)

    SET(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_LIBSWSCALE_LIBRARIES})

ENDIF()

IF(FFMPEG_LIBSWRESAMPLE_FOUND)
    INCLUDE_DIRECTORIES( ${FFMPEG_LIBSWRESAMPLE_INCLUDE_DIRS} ${FFMPEG_LIBSWRESAMPLE_INCLUDE_DIRS}/libswresample )

    ADD_DEFINITIONS(-DUSE_SWRESAMPLE)

    SET(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_LIBSWRESAMPLE_LIBRARIES})
ENDIF()
IF(FFMPEG_LIBAVRESAMPLE_FOUND)
    INCLUDE_DIRECTORIES( ${FFMPEG_LIBAVRESAMPLE_INCLUDE_DIRS} ${FFMPEG_LIBAVRESAMPLE_INCLUDE_DIRS}/libavresample )

    ADD_DEFINITIONS(-DUSE_AVRESAMPLE)

    SET(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_LIBAVRESAMPLE_LIBRARIES})
ENDIF()

SET(TARGET_EXTERNAL_LIBRARIES ${FFMPEG_LIBRARIES} )

ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS)

# MESSAGE("FFMPEG_LIBAVFORMAT_INCLUDE_DIRS = "  ${FFMPEG_LIBAVFORMAT_INCLUDE_DIRS} )
# MESSAGE("FFMPEG_LIBAVDEVICE_INCLUDE_DIRS = "  ${FFMPEG_LIBAVDEVICE_INCLUDE_DIRS} )
# MESSAGE("FFMPEG_LIBAVCODEC_INCLUDE_DIRS = "  ${FFMPEG_LIBAVCODEC_INCLUDE_DIRS} )
# MESSAGE("FFMPEG_LIBAVUTIL_INCLUDE_DIRS = "  ${FFMPEG_LIBAVUTIL_INCLUDE_DIRS} )
# MESSAGE("FFMPEG_LIBRARIES = "  ${FFMPEG_LIBRARIES} )

SET(TARGET_SRC
    FFmpegClocks.cpp
    FFmpegDecoderAudio.cpp
    FFmpegDecoder.cpp
    FFmpegDecoderVideo.cpp
    FFmpegImageStream.cpp
    FFmpegAudioStream.cpp
    FFmpegParameters.cpp
    ReaderWriterFFmpeg.cpp
)

SET(TARGET_H
    BoundedMessageQueue.hpp
    FFmpegClocks.hpp
    FFmpegDecoderAudio.hpp
    FFmpegDecoder.hpp
    FFmpegDecoderVideo.hpp
    FFmpegHeaders.hpp
    FFmpegPacket.hpp
    FFmpegImageStream.hpp
    FFmpegAudioStream.hpp
    FFmpegParameters.hpp
    MessageQueue.hpp
)

IF(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
ENDIF()

#### end var setup  ###
SETUP_PLUGIN(ffmpeg ffmpeg)

IF(MSVC)
    SET_TARGET_PROPERTIES(${TARGET_DEFAULT_PREFIX}ffmpeg PROPERTIES LINK_FLAGS "/OPT:NOREF")
ENDIF(MSVC)
