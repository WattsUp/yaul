#ifndef _YAUL_EXAMPLE_LOGGING_HPP_
#define _YAUL_EXAMPLE_LOGGING_HPP_

#include <spdlog/spdlog.h>
#include <yaul/yaul.hpp>

void initializeLogFile(const char* filename);
void libyaulLog(::yaul::LogLevel level, const char* msg) noexcept;

#endif /* _YAUL_EXAMPLE_LOGGING_HPP_ */