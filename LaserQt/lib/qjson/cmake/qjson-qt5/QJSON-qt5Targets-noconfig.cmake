#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "qjson-qt5" for configuration ""
set_property(TARGET qjson-qt5 APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(qjson-qt5 PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "Qt5::Core"
  IMPORTED_LOCATION_NOCONFIG "/home/summy/Cpp-LaserQt/LaserQt/lib/libqjson-qt5.so.0.9.0"
  IMPORTED_SONAME_NOCONFIG "libqjson-qt5.so.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS qjson-qt5 )
list(APPEND _IMPORT_CHECK_FILES_FOR_qjson-qt5 "/home/summy/Cpp-LaserQt/LaserQt/lib/libqjson-qt5.so.0.9.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
