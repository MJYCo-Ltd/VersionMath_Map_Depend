#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libjpeg-turbo::jpeg" for configuration "Debug"
set_property(TARGET libjpeg-turbo::jpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libjpeg-turbo::jpeg PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/jpegd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/jpeg62d.dll"
  )

list(APPEND _cmake_import_check_targets libjpeg-turbo::jpeg )
list(APPEND _cmake_import_check_files_for_libjpeg-turbo::jpeg "${_IMPORT_PREFIX}/lib/jpegd.lib" "${_IMPORT_PREFIX}/bin/jpeg62d.dll" )

# Import target "libjpeg-turbo::turbojpeg" for configuration "Debug"
set_property(TARGET libjpeg-turbo::turbojpeg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libjpeg-turbo::turbojpeg PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/turbojpegd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/turbojpegd.dll"
  )

list(APPEND _cmake_import_check_targets libjpeg-turbo::turbojpeg )
list(APPEND _cmake_import_check_files_for_libjpeg-turbo::turbojpeg "${_IMPORT_PREFIX}/lib/turbojpegd.lib" "${_IMPORT_PREFIX}/bin/turbojpegd.dll" )

# Import target "libjpeg-turbo::turbojpeg-static" for configuration "Debug"
set_property(TARGET libjpeg-turbo::turbojpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libjpeg-turbo::turbojpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/turbojpeg-staticd.lib"
  )

list(APPEND _cmake_import_check_targets libjpeg-turbo::turbojpeg-static )
list(APPEND _cmake_import_check_files_for_libjpeg-turbo::turbojpeg-static "${_IMPORT_PREFIX}/lib/turbojpeg-staticd.lib" )

# Import target "libjpeg-turbo::jpeg-static" for configuration "Debug"
set_property(TARGET libjpeg-turbo::jpeg-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libjpeg-turbo::jpeg-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/jpeg-staticd.lib"
  )

list(APPEND _cmake_import_check_targets libjpeg-turbo::jpeg-static )
list(APPEND _cmake_import_check_files_for_libjpeg-turbo::jpeg-static "${_IMPORT_PREFIX}/lib/jpeg-staticd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
