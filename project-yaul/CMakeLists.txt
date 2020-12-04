set (SRCS_API
  "yaul.cpp"
  "log.cpp"
)

set (SRCS
)

set (SRCS_UNIX
)

set (SRCS_WIN
)

target_sources("yaul" PRIVATE ${SRCS} ${SRCS_API} $<IF:$<BOOL:${WIN32}>, ${SRCS_WIN}, ${SRCS_UNIX}>)

set (TEST_SRCS
)

target_sources("gtester" PRIVATE ${TEST_SRCS} ${SRCS})