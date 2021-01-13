#include "logging.h"

#include <spdlog/sinks/rotating_file_sink.h>

static std::shared_ptr<spdlog::logger> libyaulLogger = nullptr;

void initializeLogFile(const char* filename) {
  constexpr size_t maxFileSize  = 1024 * 1024 * 5;
  constexpr size_t maxFileCount = 3;
  auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
      filename, maxFileSize, maxFileCount, false);
  libyaulLogger  = std::make_shared<spdlog::logger>("libyaul", fileSink);
  auto appLogger = std::make_shared<spdlog::logger>("example", fileSink);
  spdlog::set_default_logger(appLogger);
}

void libyaulLog(::yaul::LogLevel level, const char* msg) noexcept {
  libyaulLogger->log(static_cast<spdlog::level::level_enum>(level), msg);
}