#include "logging.hpp"

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

namespace common::logging {

spdlog::sink_ptr sinkFile    = nullptr;
spdlog::sink_ptr sinkConsole = nullptr;

/**
 * @brief Configure logging with a file, and a console for debug builds
 *
 * @param filename of the log file, nullptr for no logging to file. Configures
 * level to >=debug or >=info if a release build
 */
void configure(const char* filename) {
  sinkFile    = nullptr;
  sinkConsole = nullptr;
  std::vector<spdlog::sink_ptr> sinks;

#ifdef DEBUG
  sinkConsole = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  sinkConsole->set_level(spdlog::level::warn);
  sinks.emplace_back(sinkConsole);
#endif /* DEBUG */

  // Create a file sink
  if (filename != nullptr) {
    sinkFile = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        filename, MAX_FILE_SIZE, MAX_FILE_COUNT);
#ifdef DEBUG
    sinkFile->set_level(spdlog::level::debug);
#else  /* !DEBUG */
    sinkFile->set_level(spdlog::level::info);
#endif /* DEBUG */
    sinks.emplace_back(sinkFile);
  }

  std::shared_ptr<spdlog::logger> logger =
      std::make_shared<spdlog::logger>("", begin(sinks), end(sinks));
  spdlog::set_default_logger(logger);

  spdlog::set_pattern("[%P:%t:%m%d/%H:%M:%S.%e][%^%=7l%$] %v");

  // Set logger level to lowest so the sinks decide which level to log
  spdlog::set_level(spdlog::level::trace);
}
}  // namespace common::logging