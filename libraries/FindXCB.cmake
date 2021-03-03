# - FindXCB
#
# Copyright 2015 Valve Coporation (modified)

find_package(PkgConfig)

if(NOT XCB_FIND_COMPONENTS)
  set(XCB_FIND_COMPONENTS xcb)
endif()

include(FindPackageHandleStandardArgs)
set(XCB_FOUND true)
set(XCB_INCLUDE_DIRS "")
set(XCB_LIBRARIES "")
foreach(comp ${XCB_FIND_COMPONENTS})
  # component name
  string(TOUPPER ${comp} compname)
  string(REPLACE "-" "_" compname "${compname}")
  # header name
  string(REPLACE "xcb-" "" XCB_${compname}_HEADER "xcb/${comp}.h")

  # library name
  set(XCB_${compname}_LIB "xcb-${comp}")

  # Overrides
  set(XCB_XCB_LIB "xcb")
  set(XCB_EWMH_HEADER "xcb/xcb_ewmh.h")
  set(XCB_ICCCM_HEADER "xcb/xcb_icccm.h")

  pkg_check_modules(PC_${comp} QUIET ${comp})

  find_path(${compname}_INCLUDE_DIR NAMES ${XCB_${compname}_HEADER}
    HINTS
    ${PC_${comp}_INCLUDEDIR}
    ${PC_${comp}_INCLUDE_DIRS}
    )

  find_library(${compname}_LIBRARY NAMES ${XCB_${compname}_LIB}
    HINTS
    ${PC_${comp}_LIBDIR}
    ${PC_${comp}_LIBRARY_DIRS}
    )

  set(FPHSA_NAME_MISMATCHED TRUE)
  find_package_handle_standard_args(${comp}
    FOUND_VAR ${comp}_FOUND
    REQUIRED_VARS ${compname}_INCLUDE_DIR ${compname}_LIBRARY)
  mark_as_advanced(${compname}_INCLUDE_DIR ${compname}_LIBRARY)

  list(APPEND XCB_INCLUDE_DIRS ${${compname}_INCLUDE_DIR})
  list(APPEND XCB_LIBRARIES ${${compname}_LIBRARY})

  if(NOT ${comp}_FOUND)
    set(XCB_FOUND false)
  else()
    add_library(XCB::${comp} INTERFACE IMPORTED)
    set_target_properties(XCB::${comp} PROPERTIES
      INTERFACE_LINK_LIBRARIES "${${compname}_LIBRARY}"
    )
    target_include_directories(XCB::${comp} SYSTEM INTERFACE ${${compname}_INCLUDE_DIR})
  endif()
endforeach()

list(REMOVE_DUPLICATES XCB_INCLUDE_DIRS)