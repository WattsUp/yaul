#include <yaul/common.hpp>

#include "common/logger.hpp"
#include "common/version.h"

#include <miniz.h>

namespace yaul {

YAUL_API const char* getVersionString() {
  return VERSION_STRING;
}

YAUL_API const char* getVersionStringFull() {
  return VERSION_STRING_FULL;
}

Result::Result() noexcept = default;

/**
 * @brief Construct a new unsuccessful Result object
 * Contains a message explaining failure
 *
 * @param message explation message to copy into result
 */
Result::Result(const char* message) noexcept {
  size_t len = strlen(message) + 1;
  // NOLINTNEXTLINE (modernize-avoid-c-arrays)
  msg = std::make_unique<char[]>(len);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  errno_t err = strcpy_s(msg.get(), len, message);
  if (err != 0) {
    Logger::instance().log(LogLevel::error,
                           u8"Unknown result, failed to copy message");
  }
#else /* !WIN32 */
  strncpy(msg.get(), message, len);
#endif /* !WIN32 */
}

Result::~Result() noexcept = default;

Result::Result(Result&& o) noexcept : msg(std::move(o.msg)) {}

Result& Result::operator=(Result&& o) noexcept {
  if (&o != this)
    msg = std::move(o.msg);
  return *this;
}

}  // namespace yaul