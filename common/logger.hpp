#ifndef YAUL_LOGGER_HPP
#define YAUL_LOGGER_HPP

#include <yaul/common.hpp>

#include "string.hpp"

namespace yaul {

class Logger {
 private:
  Logger()                = default;
  logger_t loggerFunction = nullptr;

 public:
  ~Logger() = default;
  YAUL_NO_COPY(Logger);
  YAUL_NO_MOVE(Logger);

  /**
   * @brief Get the singleton logger instance
   *
   * @return Logger&
   */
  static Logger& instance() {
    static Logger instance;
    return instance;
  }

  /**
   * @brief Log a message to the user provided logging function
   *
   * @param level to log message
   * @param msg to log
   */
  inline void log(LogLevel level, const string& msg) {
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
};

}  // namespace yaul

#endif /* _YAUL_LOGGER_HPP_ */