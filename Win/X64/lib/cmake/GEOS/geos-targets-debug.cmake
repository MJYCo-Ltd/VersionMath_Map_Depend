#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GEOS::geos" for configuration "Debug"
set_property(TARGET GEOS::geos APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GEOS::geos PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/geosd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/geosd.dll"
  )

list(APPEND _cmake_import_check_targets GEOS::geos )
list(APPEND _cmake_import_check_files_for_GEOS::geos "${_IMPORT_PREFIX}/lib/geosd.lib" "${_IMPORT_PREFIX}/bin/geosd.dll" )

# Import target "GEOS::geos_c" for configuration "Debug"
set_property(TARGET GEOS::geos_c APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GEOS::geos_c PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/geos_cd.lib"
  IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG "GEOS::geos"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/geos_cd.dll"
  )

list(APPEND _cmake_import_check_targets GEOS::geos_c )
list(APPEND _cmake_import_check_files_for_GEOS::geos_c "${_IMPORT_PREFIX}/lib/geos_cd.lib" "${_IMPORT_PREFIX}/bin/geos_cd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
