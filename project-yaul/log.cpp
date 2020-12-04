#include "yaul/log.h"

#include "common/logging.hpp"
namespace yaul {

void log(const char* s) {
  spdlog::critical(s);
}

void configureLogging(const char* filename) {
  common::logging::configure(filename);
}

}  // namespace yaul