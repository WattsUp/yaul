#ifndef _YAUL_API_HPP_
#define _YAUL_API_HPP_

#ifdef _MSC_VER
#if YAUL_CREATE_SHARED_LIBRARY
#define YAUL_API __declspec(dllexport)
#elif YAUL_LINKED_AS_SHARED_LIBRARY
#define YAUL_API __declspec(dllimport)
#endif
#elif __GNUC__ >= 4 || defined(__clang__)
#define YAUL_API __attribute__((visibility("default")))
#endif /* _MSC_VER */

#ifndef YAUL_API
#define YAUL_API
#endif /* YAUL_API */

/* stdint.h is not available on older MSVC */
#if defined(_MSC_VER) && (_MSC_VER < 1600) && (!defined(_STDINT)) && \
    (!defined(_STDINT_H))
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#else
#include <stdint.h>
#endif

#define YAUL_NO_COPY(x) \
  x(const x&) = delete; \
  x& operator=(const x&) = delete;

#define YAUL_NO_MOVE(x)  \
  x(const x&&) = delete; \
  x& operator=(const x&&) = delete;

namespace yaul {

/********************************** Logging ***********************************/

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
typedef void (*logger_t)(LogLevel level, const char* msg) noexcept;

}  // namespace yaul

/******************************* Error Handling *******************************/
#include <cstring>
#include <exception>

namespace yaul {

class Result {
 public:
  /**
   * @brief Construct a new successful Result object
   * Contains no message
   *
   */
  Result() noexcept {}

  /**
   * @brief Construct a new unsuccessful Result object
   * Contains a message explaining failure
   *
   * @param message explation message to copy into result
   */
  explicit Result(const char* message) noexcept(false) {
    size_t len  = strlen(message) + 1;
    msg         = new char[len];  // NOLINT (cppcoreguidelines-owning-memory)
    errno_t err = strcpy_s(msg, len, message);
    if (err != 0) {
      delete[] msg;
      throw std::exception(u8"Unknown result, failed to copy message");
    }
  }

  ~Result() noexcept { delete[] msg; }

  Result(Result&& result) noexcept {
    msg        = result.msg;
    result.msg = nullptr;
  }

  Result& operator=(Result&& result) noexcept {
    if (&result == this) {
      return *this;
    }

    delete[] msg;
    msg        = result.msg;
    result.msg = nullptr;
    return *this;
  }

  YAUL_NO_COPY(Result);

  /**
   * @brief Check if this result is unsuccessful
   *
   * @return true if this result contains an explanatory message
   * @return false if this result has no message therefore successful
   */
  inline bool failed() const noexcept { return msg != nullptr; }

  inline operator char*() const noexcept { return msg; }

 private:
  char* msg = nullptr;
};

/**
 * @brief Execute a function and catch any exceptions and assign Result r to a
 * result with the exception's what()
 *
 * @param function to execute
 * @param r Result object to assign caught exceptions
 */
#define YAUL_EXCEPTION_WRAPPER_CATCH(function, r) \
  try {                                           \
    function;                                     \
  } catch (const std::exception& e) {             \
    r = Result(e.what());                         \
  }

/**
 * @brief Execute an apiFunction and check if result failed then throw an
 * exception else return function returned object
 *
 * @param returnType object to assign from function execution and return
 * @param apiFunction to execute and pass a result object into
 * @param __VA_ARGS__ arguments to pass to apiFunction before appending result
 * object
 */
#define YAUL_EXCEPTION_WRAPPER_THROW(returnType, apiFunction, ...) \
  Result r;                                                        \
  returnType o = apiFunction(__VA_ARGS__, r);                      \
  if (r.failed())                                                  \
    throw std::exception(r);                                       \
  return o;

/**
 * @brief Execute an apiFunction and check if result failed then throw an
 * exception
 *
 * @param apiFunction to execute and pass a result object into
 * @param __VA_ARGS__ arguments to pass to apiFunction before appending result
 * object *
 */
#define YAUL_EXCEPTION_WRAPPER_THROW_VOID(apiFunction, ...) \
  Result r;                                                 \
  apiFunction(__VA_ARGS__, r);                              \
  if (r.failed())                                           \
    throw std::exception(r);

}  // namespace yaul

#endif /* _YAUL_API_HPP_ */