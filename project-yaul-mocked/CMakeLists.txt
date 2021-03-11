set (SRCS_API
)

set (SRCS
)

set (SRCS_UNIX
)

set (SRCS_WIN
  "monitor_win32.cpp"
)

target_sources("yaul-mocked" PRIVATE ${SRCS} ${SRCS_API} $<IF:$<BOOL:${WIN32}>, ${SRCS_WIN}, ${SRCS_UNIX}>)

set (TEST_SRCS
)

target_sources("gtester" PRIVATE ${TEST_SRCS} ${SRCS})