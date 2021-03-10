# add_subdirectory(
#   "vg"
# )

set (SRCS_API
  "application.cpp"
  "common.cpp"
  "monitor.cpp"
  "object.cpp"
  "window.cpp"
)

set (SRCS
)

set (SRCS_UNIX
  "application_unix.cpp"
  "monitor_unix.cpp"
  "window_unix.cpp"
  "xcb_unix.cpp"
)

set (SRCS_WIN
  "application_win32.cpp"
  "monitor_win32.cpp"
  "window_win32.cpp"
)

target_sources("yaul" PRIVATE ${SRCS} ${SRCS_API} $<IF:$<BOOL:${WIN32}>, ${SRCS_WIN}, ${SRCS_UNIX}>)

set (TEST_SRCS
  "common_test.cpp"
  "dimensions_test.cpp"
  "object_test.cpp"
)

target_sources("gtester" PRIVATE ${TEST_SRCS} ${SRCS})