#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osg2vsg::osg2vsg" for configuration "Debug"
set_property(TARGET osg2vsg::osg2vsg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(osg2vsg::osg2vsg PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/osg2vsgd.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/osg2vsgd.dll"
  )

list(APPEND _cmake_import_check_targets osg2vsg::osg2vsg )
list(APPEND _cmake_import_check_files_for_osg2vsg::osg2vsg "${_IMPORT_PREFIX}/lib/osg2vsgd.lib" "${_IMPORT_PREFIX}/bin/osg2vsgd.dll" )

# Import target "osg2vsg::osgdb_vsg" for configuration "Debug"
set_property(TARGET osg2vsg::osgdb_vsg APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(osg2vsg::osgdb_vsg PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_DEBUG ""
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/osgPlugins-3.7.0/osgdb_vsgd.dll"
  )

list(APPEND _cmake_import_check_targets osg2vsg::osgdb_vsg )
list(APPEND _cmake_import_check_files_for_osg2vsg::osgdb_vsg "${_IMPORT_PREFIX}/bin/osgPlugins-3.7.0/osgdb_vsgd.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
