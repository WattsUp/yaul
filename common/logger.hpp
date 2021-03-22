#ifndef YAUL_LOGGER_HPP
#define YAUL_LOGGER_HPP

#include <yaul/common.hpp>

#include "common/string.hpp"

namespace yaul {

class Logger {
 private:
  Logger() noexcept       = default;
  logger_t loggerFunction = nullptr;

 public:
  ~Logger() noexcept = default;
  YAUL_NO_COPY(Logger);
  YAUL_NO_MOVE(Logger);

  /**
   * @brief Get the singleton logger instance
   *
   * @return Logger&
   */
  static Logger& instance() noexcept {
    static Logger instance;
    return instance;
  }

  /**
   * @brief Log a message to the user provided logging function
   *
   * @param level to log message at
   * @param msg to log
   */
  inline void log(LogLevel level, const string& msg) noexcept {
    if (loggerFunction != nullptr) {
      loggerFunction(level, msg.c_str());
    }
  }

  /**
   * @brief Set the user provided logging function
   *
   * @param logger to direct messages to
   */
  inline void setLogger(logger_t logger) { loggerFunction = logger; }

  // TODO (WattsUp) add usability functions and fmt library
};

}  // namespace yaul

#endif /* YAUL_LOGGER_HPP */