# - Find LevelDB
#
#  LEVELDB_INCLUDE - Where to find leveldb/db.h
#  LEVELDB_LIBS    - List of libraries when using LevelDB.
#  LEVELDB_FOUND   - True if LevelDB found.

get_filename_component(module_file_path ${CMAKE_CURRENT_LIST_FILE} PATH)

# Look for the header file.
find_path(LEVELDB_INCLUDE_DIR NAMES leveldb/db.h PATHS $ENV{LEVELDB_ROOT}/include /opt/local/include /usr/local/include /usr/include DOC "Path in which the file leveldb/db.h is located." )
mark_as_advanced(LEVELDB_INCLUDE_DIR)

# Look for the library.
# Does this work on UNIX systems? (LINUX)
find_library(LEVELDB_LIBRARY NAMES leveldb PATHS /usr/lib $ENV{LEVELDB_ROOT}/lib DOC "Path to leveldb library." )
mark_as_advanced(LEVELDB_LIBRARY)

# Copy the results to the output variables.
if (LEVELDB_INCLUDE_DIR AND LEVELDB_LIBRARY)
  message(STATUS "Found leveldb in ${LEVELDB_INCLUDE_DIR} ${LEVELDB_LIBRARY}")
  set(LEVELDB_FOUND 1)
  include(CheckCXXSourceCompiles)
  #set(CMAKE_REQUIRED_LIBRARY ${LEVELDB_LIBRARY} pthread)
  set(CMAKE_REQUIRED_INCLUDES ${LEVELDB_INCLUDE_DIR})
 else ()
   set(LEVELDB_FOUND 0)
 endif ()

 # Report the results.
 if (NOT LEVELDB_FOUND)
   set(LEVELDB_DIR_MESSAGE "LEVELDB was not found. Make sure LEVELDB_LIBRARY and LEVELDB_INCLUDE_DIR are set.")
   if (LEVELDB_FIND_REQUIRED)
     message(FATAL_ERROR "${LEVELDB_DIR_MESSAGE}")
   elseif (NOT LEVELDB_FIND_QUIETLY)
     message(STATUS "${LEVELDB_DIR_MESSAGE}")
   endif ()
 endif ()
