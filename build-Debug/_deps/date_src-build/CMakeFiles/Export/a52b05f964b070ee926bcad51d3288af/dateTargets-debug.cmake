#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "date::date-tz" for configuration "Debug"
set_property(
  TARGET date::date-tz
  APPEND
  PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(
  date::date-tz
  PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
             IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libdate-tz.a")

list(APPEND _cmake_import_check_targets date::date-tz)
list(APPEND _cmake_import_check_files_for_date::date-tz
     "${_IMPORT_PREFIX}/lib/libdate-tz.a")

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
