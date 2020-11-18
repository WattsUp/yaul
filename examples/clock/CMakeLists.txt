set(TARGET "clock")

add_executable(${TARGET})

set_target_properties(${TARGET}
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/examples"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/examples"
)

set (SRCS
  "main.cpp"
)

set (SRCS_UNIX
)

set (SRCS_WIN
)

target_sources(${TARGET} PRIVATE ${SRCS} $<IF:$<BOOL:${WIN32}>, ${SRCS_WIN}, ${SRCS_UNIX}>)
target_link_libraries(${TARGET} PRIVATE "yaul")
