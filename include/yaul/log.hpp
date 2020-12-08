#ifndef _YAUL_LOG_H_
#define _YAUL_LOG_H_

#include "api.hpp"

namespace yaul {

enum class LogLevel : uint8_t {
  debug    = 1,
  info     = 2,
  warning  = 3,
  error    = 4,
  critical = 5,
  off      = 6
};

/**
 * @brief Log a message from yaul library
 *
 * @param level of the log statement
 * @param msg to log
 */
typedef void (*logger_t)(LogLevel level, const char* msg);

}  // namespace yaul

#endif /* _YAUL_LOG_H_ */