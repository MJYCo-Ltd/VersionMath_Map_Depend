INCLUDE_DIRECTORIES(${OSG_INCLUDE_DIRS} ${QT_INCLUDES})

SET(MOC_HDRS
)

IF(Qt5Widgets_FOUND)
    QT5_WRAP_CPP( MOC_SRCS ${MOC_HDRS} OPTIONS "-f" )
	SET(TARGET_ADDED_LIBRARIES
        osgEarthQt
    )
ELSE()
    INCLUDE( ${QT_USE_FILE} )
    QT4_WRAP_CPP( MOC_SRCS ${MOC_HDRS} OPTIONS "-f" )
	SET(TARGET_ADDED_LIBRARIES
        osgEarthQt
        ${QT_QTCORE_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTOPENGL_LIBRARY}
    )
ENDIF()

SET(TARGET_H
    ${LIB_QT_RCS}
)

SET(TARGET_SRC
    ${MOC_SRCS}
    ${LIB_RC_SRCS}
    osgearth_demo.cpp
)

#### end var setup  ###
SETUP_APPLICATION(osgearth_demo)
