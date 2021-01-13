set (SRCS
  "main.cpp"
  "logging.cpp"
)

set (SRCS_UNIX
)

set (SRCS_WIN
)

target_sources("3d-and-vg" PRIVATE ${SRCS} $<IF:$<BOOL:${WIN32}>, ${SRCS_WIN}, ${SRCS_UNIX}>)

find_package(spdlog REQUIRED)
target_link_libraries("3d-and-vg" PRIVATE spdlog::spdlog_header_only)