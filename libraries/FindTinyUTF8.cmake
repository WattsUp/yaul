find_package(TinyUTF8 CONFIG QUIET)

if(TinyUTF8_FOUND)
  # message(STATUS "Found TinyUTF8: ${TinyUTF8_DIR}")
else()
  find_path(TINYUTF8_INCLUDE_DIR NAMES tinyutf8.h)
  find_library(TINYUTF8_LIBRARY tinyutf8)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(TinyUTF8 DEFAULT_MSG TINYUTF8_LIBRARY TINYUTF8_INCLUDE_DIR)
  mark_as_advanced(TINYUTF8_INCLUDE_DIR TINYUTF8_LIBRARY)

  if(TinyUTF8_FOUND AND NOT TARGET tinyutf8::tinyutf8)
    add_library(tinyutf8::tinyutf8 UNKNOWN IMPORTED)
    set_target_properties(tinyutf8::tinyutf8 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${TINYUTF8_INCLUDE_DIR}")
    set_property(TARGET tinyutf8::tinyutf8 APPEND PROPERTY IMPORTED_LOCATION "${TINYUTF8_LIBRARY}")
  endif()
endif()
