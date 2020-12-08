set (SRCS
)

foreach(target ${TARGETS})
  target_sources(${target} PRIVATE ${SRCS})
endforeach()

set (TEST_SRCS
  "main_test.cpp"
)

target_sources("gtester" PRIVATE ${TEST_SRCS})
target_link_libraries("gtester" PRIVATE "yaul")
