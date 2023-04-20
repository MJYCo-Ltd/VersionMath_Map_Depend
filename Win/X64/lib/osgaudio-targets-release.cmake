#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osgAudio" for configuration "Release"
set_property(TARGET osgAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osgAudio PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/osgAudio.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "D:/Git/OpenSource/BuildOK/lib/alut.lib;D:/Git/OpenSource/BuildOK/lib/OpenAL32.lib;D:/Git/OpenSource/BuildOK/lib/ogg.lib;D:/Git/OpenSource/BuildOK/lib/vorbis.lib;D:/Git/OpenSource/BuildOK/lib/vorbisfile.lib;D:/Git/OpenSource/BuildOK/lib/osg.lib;D:/Git/OpenSource/BuildOK/lib/osgDB.lib;D:/Git/OpenSource/BuildOK/lib/osgUtil.lib;D:/Git/OpenSource/BuildOK/lib/osgGA.lib;D:/Git/OpenSource/BuildOK/lib/osgText.lib;D:/Git/OpenSource/BuildOK/lib/osgViewer.lib;D:/Git/OpenSource/BuildOK/lib/OpenThreads.lib;openalpp"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/osgAudio.dll"
  )

list(APPEND _cmake_import_check_targets osgAudio )
list(APPEND _cmake_import_check_files_for_osgAudio "${_IMPORT_PREFIX}/lib/osgAudio.lib" "${_IMPORT_PREFIX}/bin/osgAudio.dll" )

# Import target "osgdb_osgAudio" for configuration "Release"
set_property(TARGET osgdb_osgAudio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osgdb_osgAudio PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/osgdb_osgAudio.dll"
  )

list(APPEND _cmake_import_check_targets osgdb_osgAudio )
list(APPEND _cmake_import_check_files_for_osgdb_osgAudio "${_IMPORT_PREFIX}/bin/osgdb_osgAudio.dll" )

# Import target "openalpp" for configuration "Release"
set_property(TARGET openalpp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(openalpp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/openalpp.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "D:/Git/OpenSource/BuildOK/lib/alut.lib;D:/Git/OpenSource/BuildOK/lib/OpenAL32.lib;D:/Git/OpenSource/BuildOK/lib/ogg.lib;D:/Git/OpenSource/BuildOK/lib/vorbis.lib;D:/Git/OpenSource/BuildOK/lib/vorbisfile.lib;D:/Git/OpenSource/BuildOK/lib/osg.lib;D:/Git/OpenSource/BuildOK/lib/osgDB.lib;D:/Git/OpenSource/BuildOK/lib/osgUtil.lib;D:/Git/OpenSource/BuildOK/lib/osgGA.lib;D:/Git/OpenSource/BuildOK/lib/osgText.lib;D:/Git/OpenSource/BuildOK/lib/osgViewer.lib;D:/Git/OpenSource/BuildOK/lib/OpenThreads.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/openalpp.dll"
  )

list(APPEND _cmake_import_check_targets openalpp )
list(APPEND _cmake_import_check_files_for_openalpp "${_IMPORT_PREFIX}/lib/openalpp.lib" "${_IMPORT_PREFIX}/bin/openalpp.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
