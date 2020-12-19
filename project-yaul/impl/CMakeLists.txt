set (SRCS
  "application.cpp"
  "window.cpp"
)

set (SRCS_UNIX
)

set (SRCS_WIN
  "window_win32.cpp"
)

target_sources("yaul" PRIVATE ${SRCS} $<IF:$<BOOL:${WIN32}>, ${SRCS_WIN}, ${SRCS_UNIX}>)

set (TEST_SRCS
)

target_sources("gtester" PRIVATE ${TEST_SRCS} ${SRCS})