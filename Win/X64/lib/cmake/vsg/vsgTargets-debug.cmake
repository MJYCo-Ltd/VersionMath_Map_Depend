#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "vsg::vsg" for configuration "Debug"
set_property(TARGET vsg::vsg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(vsg::vsg PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/vsgd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/vsg-12d.dll"
  )

list(APPEND _cmake_import_check_targets vsg::vsg )
list(APPEND _cmake_import_check_files_for_vsg::vsg "${_IMPORT_PREFIX}/lib/vsgd.lib" "${_IMPORT_PREFIX}/bin/vsg-12d.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
