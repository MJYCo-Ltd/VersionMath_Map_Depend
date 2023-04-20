#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osgAudio" for configuration "Debug"
set_property(TARGET osgAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(osgAudio PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/osgAudiod.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "D:/Git/OpenSource/BuildOK/lib/alutd.lib;D:/Git/OpenSource/BuildOK/lib/OpenAL32.lib;D:/Git/OpenSource/BuildOK/lib/oggd.lib;D:/Git/OpenSource/BuildOK/lib/vorbisd.lib;D:/Git/OpenSource/BuildOK/lib/vorbisfiled.lib;D:/Git/OpenSource/BuildOK/lib/osgd.lib;D:/Git/OpenSource/BuildOK/lib/osgDBd.lib;D:/Git/OpenSource/BuildOK/lib/osgUtild.lib;D:/Git/OpenSource/BuildOK/lib/osgGAd.lib;D:/Git/OpenSource/BuildOK/lib/osgTextd.lib;D:/Git/OpenSource/BuildOK/lib/osgViewerd.lib;D:/Git/OpenSource/BuildOK/lib/OpenThreadsd.lib;openalpp"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/osgAudiod.dll"
  )

list(APPEND _cmake_import_check_targets osgAudio )
list(APPEND _cmake_import_check_files_for_osgAudio "${_IMPORT_PREFIX}/lib/osgAudiod.lib" "${_IMPORT_PREFIX}/bin/osgAudiod.dll" )

# Import target "osgdb_osgAudio" for configuration "Debug"
set_property(TARGET osgdb_osgAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(osgdb_osgAudio PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/osgdb_osgAudiod.dll"
  )

list(APPEND _cmake_import_check_targets osgdb_osgAudio )
list(APPEND _cmake_import_check_files_for_osgdb_osgAudio "${_IMPORT_PREFIX}/bin/osgdb_osgAudiod.dll" )

# Import target "openalpp" for configuration "Debug"
set_property(TARGET openalpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(openalpp PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/openalppd.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "D:/Git/OpenSource/BuildOK/lib/alutd.lib;D:/Git/OpenSource/BuildOK/lib/OpenAL32.lib;D:/Git/OpenSource/BuildOK/lib/oggd.lib;D:/Git/OpenSource/BuildOK/lib/vorbisd.lib;D:/Git/OpenSource/BuildOK/lib/vorbisfiled.lib;D:/Git/OpenSource/BuildOK/lib/osgd.lib;D:/Git/OpenSource/BuildOK/lib/osgDBd.lib;D:/Git/OpenSource/BuildOK/lib/osgUtild.lib;D:/Git/OpenSource/BuildOK/lib/osgGAd.lib;D:/Git/OpenSource/BuildOK/lib/osgTextd.lib;D:/Git/OpenSource/BuildOK/lib/osgViewerd.lib;D:/Git/OpenSource/BuildOK/lib/OpenThreadsd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/openalppd.dll"
  )

list(APPEND _cmake_import_check_targets openalpp )
list(APPEND _cmake_import_check_files_for_openalpp "${_IMPORT_PREFIX}/lib/openalppd.lib" "${_IMPORT_PREFIX}/bin/openalppd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
