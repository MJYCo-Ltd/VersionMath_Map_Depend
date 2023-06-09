# This file should only be included when using Pthreads

INCLUDE (CheckFunctionExists)
INCLUDE (CheckLibraryExists)
INCLUDE (CheckSymbolExists)
INCLUDE (CheckCXXSourceCompiles)

SET(LIB_NAME OpenThreads)
SET(TARGET_H ${OpenThreads_PUBLIC_HEADERS})

SET(TARGET_SRC
   PThread.cpp
    PThreadBarrier.cpp
    PThreadBarrierPrivateData.h
    PThreadCondition.cpp
    PThreadConditionPrivateData.h
    PThreadMutex.cpp
    PThreadMutexPrivateData.h
    PThreadPrivateData.h
    ../common/Version.cpp
    ../common/Atomic.cpp
)

IF(CYGWIN)  # define for pthread_{yield,getconcurrency,setconcurrency}
    ADD_DEFINITIONS(-D_GNU_SOURCE)
ENDIF()

IF(ANDROID)
      ADD_DEFINITIONS(-D_GNU_SOURCE)
      SET(CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS} -D_GNU_SOURCE")
    SET(MODULE_USER_STATIC_OR_DYNAMIC ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC})
    #SET(CPP_EXTENSION "c++")
    SETUP_LIBRARY(${LIB_NAME})
ELSE()

    # should check?
    ADD_DEFINITIONS(-DHAVE_PTHREAD_TESTCANCEL)
    ADD_DEFINITIONS(-DHAVE_PTHREAD_CANCEL)
    ADD_DEFINITIONS(-DHAVE_PTHREAD_SETCANCELSTATE)

    SET(CMAKE_REQUIRED_LIBRARIES_SAFE "${CMAKE_REQUIRED_LIBRARIES}")
    SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

    CHECK_FUNCTION_EXISTS(pthread_yield HAVE_PTHREAD_YIELD)
    IF(HAVE_PTHREAD_YIELD)
      ADD_DEFINITIONS(-DHAVE_PTHREAD_YIELD)
    ELSE()
          # sched_yield appears not in libc, pthreads or whatever on some systems
        CHECK_FUNCTION_EXISTS(sched_yield HAVE_SCHED_YIELD)
          IF(NOT HAVE_SCHED_YIELD)
            CHECK_LIBRARY_EXISTS(rt sched_yield "" HAVE_SCHED_YIELD)
            IF(HAVE_SCHED_YIELD)
                  SET(CMAKE_THREAD_LIBS_INIT "${CMAKE_THREAD_LIBS_INIT} -lrt")
                  SET(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES_SAFE} ${CMAKE_THREAD_LIBS_INIT})
            ENDIF()
          ENDIF()
          IF(HAVE_SCHED_YIELD)
            ADD_DEFINITIONS(-DHAVE_SCHED_YIELD)
          ENDIF()
    ENDIF()

    IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
          # need to have that for pthread_setaffinity_np on linux
          ADD_DEFINITIONS(-D_GNU_SOURCE)
          SET(CMAKE_REQUIRED_DEFINITIONS "${CMAKE_REQUIRED_DEFINITIONS} -D_GNU_SOURCE")
    ENDIF()

    CHECK_FUNCTION_EXISTS(pthread_setconcurrency HAVE_PTHREAD_SETCONCURRENCY)
    IF(HAVE_PTHREAD_SETCONCURRENCY)
          ADD_DEFINITIONS(-DHAVE_PTHREAD_SETCONCURRENCY)
    ENDIF()

    CHECK_FUNCTION_EXISTS(pthread_getconcurrency HAVE_PTHREAD_GETCONCURRENCY)
    IF(HAVE_PTHREAD_GETCONCURRENCY)
          ADD_DEFINITIONS(-DHAVE_PTHREAD_GETCONCURRENCY)
    ENDIF()

    CHECK_FUNCTION_EXISTS(pthread_setaffinity_np HAVE_PTHREAD_SETAFFINITY_NP)
    IF(HAVE_PTHREAD_SETAFFINITY_NP)
          # double check that pthread_setaffinity_np is available as FreeBSD header doesn't contain required function
          CHECK_CXX_SOURCE_COMPILES("
            #include <pthread.h>
            #ifdef __FreeBSD__
            #include <pthread_np.h>
            #endif
            int main() {
            #ifdef __FreeBSD__
            cpuset_t cpumask;
            #else
            cpu_set_t cpumask;
            #endif
            CPU_ZERO( &cpumask );
            CPU_SET( 0, &cpumask );
            pthread_setaffinity_np( pthread_self(), sizeof(cpumask), &cpumask);
            return 0;
            }" COMPILES_PTHREAD_SETAFFINITY_NP)

        IF (NOT COMPILES_PTHREAD_SETAFFINITY_NP)
            SET(HAVE_PTHREAD_SETAFFINITY_NP OFF)
        ENDIF()
    ENDIF()

    IF(HAVE_PTHREAD_SETAFFINITY_NP)
          ADD_DEFINITIONS(-DHAVE_PTHREAD_SETAFFINITY_NP)
    ELSE()
          CHECK_CXX_SOURCE_COMPILES("
            #include <sched.h>
            int main() {
            cpu_set_t cpumask;
            sched_setaffinity( 0, sizeof(cpumask), &cpumask );
            return 0;
            }" HAVE_THREE_PARAM_SCHED_SETAFFINITY)
          IF(HAVE_THREE_PARAM_SCHED_SETAFFINITY)
            ADD_DEFINITIONS(-DHAVE_THREE_PARAM_SCHED_SETAFFINITY)
          ELSE()
            CHECK_CXX_SOURCE_COMPILES("
                #include <sched.h>
                int main() {
                  cpu_set_t cpumask;
                  sched_setaffinity( 0, &cpumask );
                  return 0;
                }" HAVE_TWO_PARAM_SCHED_SETAFFINITY)
            IF(HAVE_TWO_PARAM_SCHED_SETAFFINITY)
                  ADD_DEFINITIONS(-DHAVE_TWO_PARAM_SCHED_SETAFFINITY)
            ENDIF()
          ENDIF()
    ENDIF()

    ADD_LIBRARY(${LIB_NAME}
        ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
        ${TARGET_H}
        ${TARGET_SRC}
    )

    TARGET_INCLUDE_DIRECTORIES(${LIB_NAME}
        PUBLIC
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${INSTALL_INCDIR}>
    )

    IF(OPENTHREADS_SONAMES)
          SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES VERSION ${OPENTHREADS_VERSION} SOVERSION ${OPENTHREADS_SOVERSION})
    ENDIF()

    IF(APPLE)
        IF(OSG_BUILD_PLATFORM_IPHONE)
            SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE ${IPHONE_ENABLE_BITCODE})
        ENDIF()
    ENDIF()

    SET(CMAKE_REQUIRED_LIBRARIES "${CMAKE_REQUIRED_LIBRARIES_SAFE}")

    TARGET_LINK_LIBRARIES(${LIB_NAME}
        ${CMAKE_THREAD_LIBS_INIT}
    )

    # Since we're building different platforms binaries in 
    # their respective directories, we need to set the 
    # link directory so it can find this location.
    LINK_DIRECTORIES(
        ${CMAKE_CURRENT_BINARY_DIR}
    )

    IF(OSG_EXPORT_TARGETS)

        INSTALL(
            TARGETS ${LIB_NAME}
            EXPORT  ${LIB_NAME}
            ARCHIVE DESTINATION ${OSG_INSTALL_LIBDIR} COMPONENT libopenthreads-dev
            LIBRARY DESTINATION ${OSG_INSTALL_LIBDIR} COMPONENT libopenthreads
            RUNTIME DESTINATION bin COMPONENT libopenthreads
        )

        INSTALL(
            EXPORT ${LIB_NAME}
            NAMESPACE ${PKG_NAMESPACE}::
            DESTINATION ${INSTALL_CONFIGDIR}
            FILE ${LIB_NAME}-targets.cmake
            COMPONENT libopenthreads-dev
        )

        # Intentionally blank
        SET(COMPONENT_CMAKE_DEPENDENCIES)

        SET(TARGET_IMPORT_INPUT_FILE  "${PROJECT_SOURCE_DIR}/packaging/cmake/openscenegraph-component-import-targets.cmake")
        SET(TARGET_IMPORT_OUTPUT_FILE "${PROJECT_BINARY_DIR}/packaging/cmake/openscenegraph-${LIB_NAME}-import-targets.cmake")
        CONFIGURE_FILE(
            ${TARGET_IMPORT_INPUT_FILE}
            ${TARGET_IMPORT_OUTPUT_FILE}
            @ONLY
        )
        INSTALL(
            FILES ${TARGET_IMPORT_OUTPUT_FILE}
            DESTINATION ${INSTALL_CONFIGDIR}
            COMPONENT libopenscenegraph-dev
        )

    ELSE()

        INSTALL(
            TARGETS ${LIB_NAME}
            ARCHIVE DESTINATION ${OSG_INSTALL_LIBDIR} COMPONENT libopenthreads-dev
            LIBRARY DESTINATION ${OSG_INSTALL_LIBDIR} COMPONENT libopenthreads
            RUNTIME DESTINATION bin COMPONENT libopenthreads
        )

    ENDIF()

    IF(NOT OSG_COMPILE_FRAMEWORKS)

        INSTALL(
            FILES ${OpenThreads_PUBLIC_HEADERS}
            DESTINATION ${INSTALL_INCDIR}/OpenThreads
            COMPONENT libopenthreads-dev
        )

    ELSE()
           MESSAGE("Will compile OpenThreads.framework!")
        SET_TARGET_PROPERTIES(${LIB_NAME} PROPERTIES
             FRAMEWORK TRUE
             FRAMEWORK_VERSION ${OPENTHREADS_SOVERSION}
             PUBLIC_HEADER  "${OpenThreads_PUBLIC_HEADERS}"
             INSTALL_NAME_DIR "${OSG_COMPILE_FRAMEWORKS_INSTALL_NAME_DIR}"
        )
    ENDIF()
ENDIF()
#commented out# INCLUDE(ModuleInstall OPTIONAL)
