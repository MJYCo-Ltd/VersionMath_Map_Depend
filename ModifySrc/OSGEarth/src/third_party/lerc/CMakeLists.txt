cmake_minimum_required (VERSION 3.11)
project (Lerc)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
    "src/LercLib/*"
    "src/LercLib/Lerc1Decode/*"
    "include"
)
message(${SOURCES})

add_library(LercLib SHARED ${SOURCES})

target_link_libraries (LercLib)

install(
    TARGETS LercLib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION  ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES "include/Lerc_types.h" "include/Lerc_c_api.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
