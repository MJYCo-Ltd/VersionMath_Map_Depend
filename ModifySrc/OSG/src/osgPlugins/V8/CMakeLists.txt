SET(TARGET_H
    V8ScriptEngine.h
)
SET(TARGET_SRC
    V8ScriptEngine.cpp
    ReaderWriterV8.cpp
)

SET(TARGET_EXTERNAL_LIBRARIES ${TARGET_EXTERNAL_LIBRARIES} ${V8_LIBRARY})
INCLUDE_DIRECTORIES(${V8_INCLUDE_DIR} )

#### end var setup  ###
SETUP_PLUGIN(v8)
