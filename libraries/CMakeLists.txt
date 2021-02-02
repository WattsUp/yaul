set ( LIBRARY_INCLUDES
  "miniz"
)

target_sources("installer-win" PRIVATE "miniz/miniz.c")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(fmt CONFIG REQUIRED)
find_package(TinyXML2 REQUIRED)
find_package(TinyUTF8 REQUIRED)
if (UNIX)
  find_package(X11 REQUIRED)
  find_package(XCB REQUIRED COMPONENTS xcb randr)
endif()

find_package(GTest CONFIG REQUIRED)
target_link_libraries("gtester" PRIVATE GTest::gtest)

foreach(target ${TARGETS})
  target_include_directories(${target} SYSTEM PRIVATE ${LIBRARY_INCLUDES})
  target_link_libraries(${target} PRIVATE tinyxml2::tinyxml2 tinyutf8::tinyutf8)
  if (WIN32 AND (NOT BUILD_SHARED_LIBS OR ${target} STREQUAL "installer-win"))
    # Self extracting Windows installer always use static linking
    target_link_libraries(${target} PRIVATE fmt::fmt-header-only)
  endif()
  if (UNIX)
    target_link_libraries(${target} PRIVATE X11::X11 X11::X11_xcb XCB::xcb XCB::randr)
  endif()
endforeach()