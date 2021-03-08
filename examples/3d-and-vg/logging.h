#ifndef YAUL_EXAMPLE_LOGGING_HPP
#define YAUL_EXAMPLE_LOGGING_HPP

#include <spdlog/spdlog.h>
#include <yaul/yaul.hpp>

void initializeLogFile(const char* filename);
void libyaulLog(::yaul::LogLevel level, const char* msg) noexcept;

#endif /* YAUL_EXAMPLE_LOGGING_HPP */