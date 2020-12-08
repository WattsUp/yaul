#ifndef _YAUL_LOGGER_HPP_
#define _YAUL_LOGGER_HPP_

#include "yaul/log.hpp"

namespace yaul {

class Logger {
 private:
  Logger() {}
  logger_t loggerFunction = nullptr;

 public:
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger**)                 = delete;
  Logger& operator=(Logger&&) = delete;

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
  inline void log(LogLevel level, const char* msg) {
    if (instance().loggerFunction != nullptr) {
      instance().loggerFunction(level, msg);
    }
  }

  /**
   * @brief Set the user provided logging function
   *
   * @param logger to direct messages to
   */
  inline void setLogger(logger_t logger) { instance().loggerFunction = logger; }
};

}  // namespace yaul

#endif /* _YAUL_LOGGER_HPP_ */