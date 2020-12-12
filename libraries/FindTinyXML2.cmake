find_package(TinyXML2 CONFIG QUIET)

if(TinyXML2_FOUND)
  # message(STATUS "Found TinyXML2: ${TinyXML2_DIR}")
  if(NOT TINYXML2_LIBRARY)
    # in this case, we're probably using TinyXML2 version 5.0.0 or greater
    # in which case tinyxml2 is an exported target and we should use that
    if(TARGET tinyxml2)
      set(TINYXML2_LIBRARY tinyxml2)
    elseif(TARGET tinyxml2::tinyxml2)
      set(TINYXML2_LIBRARY tinyxml2::tinyxml2)
    endif()
  endif()
else()
  find_path(TINYXML2_INCLUDE_DIR NAMES tinyxml2.h)
  find_library(TINYXML2_LIBRARY tinyxml2)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(TinyXML2 DEFAULT_MSG TINYXML2_LIBRARY TINYXML2_INCLUDE_DIR)
  mark_as_advanced(TINYXML2_INCLUDE_DIR TINYXML2_LIBRARY)

  if(TinyXML2_FOUND AND NOT TARGET tinyxml2::tinyxml2)
    add_library(tinyxml2::tinyxml2 UNKNOWN IMPORTED)
    set_target_properties(tinyxml2::tinyxml2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${TINYXML2_INCLUDE_DIR}")
    set_property(TARGET tinyxml2::tinyxml2 APPEND PROPERTY IMPORTED_LOCATION "${TINYXML2_LIBRARY}")
  endif()
endif()