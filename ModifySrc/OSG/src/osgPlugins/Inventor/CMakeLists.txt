INCLUDE(OsgMacroUtils)

SET(TARGET_SRC
    DeprecatedGeometry.cpp
    ConvertToInventor.cpp
    ConvertFromInventor.cpp
    PendulumCallback.cpp
    ReaderWriterIV.cpp
    ShuttleCallback.cpp
)
SET(TARGET_HDRS
    DeprecatedGeometry.h
    ConvertToInventor.h
    ConvertFromInventor.h
    PendulumCallback.h
    ReaderWriterIV.h
    ShuttleCallback.h
)

ADD_DEFINITIONS(-DCOIN_DLL)

INCLUDE_DIRECTORIES(${INVENTOR_INCLUDE_DIR})

SET(TARGET_ADDED_LIBRARIES ${INVENTOR_LIBRARY})

SETUP_PLUGIN(iv iv)
