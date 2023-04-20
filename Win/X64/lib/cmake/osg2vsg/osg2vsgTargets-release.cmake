#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osg2vsg::osg2vsg" for configuration "Release"
set_property(TARGET osg2vsg::osg2vsg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osg2vsg::osg2vsg PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/osg2vsg.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/osg2vsg.dll"
  )

list(APPEND _cmake_import_check_targets osg2vsg::osg2vsg )
list(APPEND _cmake_import_check_files_for_osg2vsg::osg2vsg "${_IMPORT_PREFIX}/lib/osg2vsg.lib" "${_IMPORT_PREFIX}/bin/osg2vsg.dll" )

# Import target "osg2vsg::osgdb_vsg" for configuration "Release"
set_property(TARGET osg2vsg::osgdb_vsg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osg2vsg::osgdb_vsg PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/osgPlugins-3.7.0/osgdb_vsg.dll"
  )

list(APPEND _cmake_import_check_targets osg2vsg::osgdb_vsg )
list(APPEND _cmake_import_check_files_for_osg2vsg::osgdb_vsg "${_IMPORT_PREFIX}/bin/osgPlugins-3.7.0/osgdb_vsg.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
