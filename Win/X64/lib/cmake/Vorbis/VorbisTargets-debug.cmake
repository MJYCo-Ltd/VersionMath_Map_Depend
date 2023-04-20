#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Vorbis::vorbis" for configuration "Debug"
set_property(TARGET Vorbis::vorbis APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Vorbis::vorbis PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/vorbisd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/vorbisd.dll"
  )

list(APPEND _cmake_import_check_targets Vorbis::vorbis )
list(APPEND _cmake_import_check_files_for_Vorbis::vorbis "${_IMPORT_PREFIX}/lib/vorbisd.lib" "${_IMPORT_PREFIX}/bin/vorbisd.dll" )

# Import target "Vorbis::vorbisenc" for configuration "Debug"
set_property(TARGET Vorbis::vorbisenc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Vorbis::vorbisenc PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/vorbisencd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/vorbisencd.dll"
  )

list(APPEND _cmake_import_check_targets Vorbis::vorbisenc )
list(APPEND _cmake_import_check_files_for_Vorbis::vorbisenc "${_IMPORT_PREFIX}/lib/vorbisencd.lib" "${_IMPORT_PREFIX}/bin/vorbisencd.dll" )

# Import target "Vorbis::vorbisfile" for configuration "Debug"
set_property(TARGET Vorbis::vorbisfile APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Vorbis::vorbisfile PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/vorbisfiled.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/vorbisfiled.dll"
  )

list(APPEND _cmake_import_check_targets Vorbis::vorbisfile )
list(APPEND _cmake_import_check_files_for_Vorbis::vorbisfile "${_IMPORT_PREFIX}/lib/vorbisfiled.lib" "${_IMPORT_PREFIX}/bin/vorbisfiled.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
