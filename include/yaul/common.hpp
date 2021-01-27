#ifndef YAUL_COMMON_HPP
#define YAUL_COMMON_HPP

#ifdef _MSC_VER
#if YAUL_CREATE_SHARED_LIBRARY
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_API __declspec(dllexport)
#elif YAUL_LINKED_AS_SHARED_LIBRARY
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_API __declspec(dllimport)
#endif
#elif __GNUC__ >= 4 || defined(__clang__)
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_API __attribute__((visibility("default")))
#endif /* _MSC_VER */

#ifndef YAUL_API
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_API
#endif /* YAUL_API */

/* stdint.h is not available on older MSVC */
#if defined(_MSC_VER) && (_MSC_VER < 1600) && (!defined(_STDINT)) && \
    (!defined(_STDINT_H))
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
#else
#include <cstdint>
#endif

#ifdef __has_feature
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_HAS_FEATURE(x) __has_feature(x)
#else
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_HAS_FEATURE(x) 0
#endif

#if (YAUL_HAS_FEATURE(cxx_relaxed_constexpr) || __GNUC__ >= 9 || \
     _MSC_VER >= 1910)
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_CONSTEXPR constexpr
#else
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_CONSTEXPR inline
#endif

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_NO_COPY(x) \
  x(const x&) = delete; \
  x& operator=(const x&) = delete; /* NOLINT (bugprone-macro-parentheses) */

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage
#define YAUL_NO_MOVE(x)  \
  x(const x&&) = delete; \
  x& operator=(const x&&) = delete; /* NOLINT (bugprone-macro-parentheses) */

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_DEFINE_DESTRUCT(x) virtual ~x() noexcept;

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_DEFINE_MOVE(x)                                               \
  x(x&& o) noexcept;            /* NOLINT (bugprone-macro-parentheses) */ \
  x& operator=(x&& o) noexcept; /* NOLINT (bugprone-macro-parentheses) */

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_DEFINE_COPY(x) \
  x(const x& o) noexcept;   \
  x& operator=(const x& o) noexcept; /* NOLINT (bugprone-macro-parentheses) */

#include <memory>

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_DEFINE_INHERIT(x) explicit x(std::unique_ptr<Impl> p) noexcept;

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_DEFINE_INHERIT_SHARED(x) \
  explicit x(std::shared_ptr<Impl> p) noexcept;

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_DEFINE_DESTRUCT_MOVE_COPY(x) \
  YAUL_DEFINE_DESTRUCT(x) YAUL_DEFINE_MOVE(x) YAUL_DEFINE_COPY(x)

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_IMPL_DESTRUCT(x) \
  x::~x() noexcept {}

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_IMPL_MOVE(x, base)                                               \
  x::x(x&& o) noexcept /* NOLINT (bugprone-macro-parentheses) */              \
      : base(std::move(o)) {}                                                 \
  x& x::operator=(x&& o) noexcept { /* NOLINT (bugprone-macro-parentheses) */ \
    base::operator=(std::move(o));                                            \
    return *this;                                                             \
  }

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_IMPL_COPY(x, base)                                                \
  x::x(const x& o) noexcept : base(std::make_unique<Impl>(*o.impl<Impl>())) {} \
  x& x::operator= /* NOLINT (bugprone-macro-parentheses) */                    \
      (const x& o) noexcept {                                                  \
    base::operator=(o);                                                        \
    return *this;                                                              \
  }

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_IMPL_COPY_SHARED(x)                            \
  x::x(const x& o) noexcept = default;                      \
  x& x::operator=(/* NOLINT (bugprone-macro-parentheses) */ \
                  const x& o) noexcept = default;

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_IMPL_INHERIT(x) \
  x::x(x::Impl& p) noexcept : pImpl(&p) {}

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_IMPL_DESTRUCT_MOVE_COPY(x, base) \
  YAUL_IMPL_DESTRUCT(x) YAUL_IMPL_MOVE(x, base) YAUL_IMPL_COPY(x, base)

namespace yaul {

/**
 * @brief Get the library version as a string
 *
 * @return const char* [major].[minor].[patch]
 */
YAUL_API const char* getVersionString();

/**
 * @brief Get the fully qualified library version as a string
 *
 * @return const char* [major].[minor].[patch]+['~' if modified][# commits
 * ahead].[git SHA]
 */
YAUL_API const char* getVersionStringFull();

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
using logger_t = void (*)(LogLevel, const char*) noexcept;

}  // namespace yaul

/******************************* Error Handling *******************************/
#include <exception>
#include <memory>
#include <stdexcept>

namespace yaul {

class YAUL_API Result {
 public:
  /**
   * @brief Construct a new successful Result object
   * Contains no message
   *
   */
  Result() noexcept;

  /**
   * @brief Construct a new unsuccessful Result object
   * Contains a message explaining failure
   *
   * @param message explation message to copy into result
   */
  explicit Result(const char* message) noexcept;

  YAUL_DEFINE_DESTRUCT(Result);
  YAUL_DEFINE_MOVE(Result);
  YAUL_NO_COPY(Result);

  /**
   * @brief Check if this result is unsuccessful
   *
   * @return true if this result contains an explanatory message
   * @return false if this result has no message therefore successful
   */
  [[nodiscard]] inline bool failed() const noexcept {
    return static_cast<bool>(msg);
  }

  inline explicit operator char*() const noexcept { return msg.get(); }

 private:
  std::unique_ptr<char[]> msg;  // NOLINT (modernize-avoid-c-arrays)
};

/**
 * @brief Execute a function and catch any exceptions and assign Result r to a
 * result with the exception's what()
 *
 * @param function to execute
 */
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_EXCEPTION_WRAPPER_CATCH(function) \
  try {                                        \
    function;                                  \
  } catch (const std::exception& e) {          \
    *(r) = Result(e.what());                   \
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
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_EXCEPTION_WRAPPER_THROW(returnType, apiFunction, ...) \
  Result r;                                                        \
  returnType o{apiFunction(__VA_ARGS__, &r)};                      \
  if (r.failed())                                                  \
    throw std::runtime_error(static_cast<char*>(r));               \
  return o;

/**
 * @brief Execute an apiFunction and check if result failed then throw an
 * exception
 *
 * @param apiFunction to execute and pass a result object into
 * @param __VA_ARGS__ arguments to pass to apiFunction before appending result
 * object *
 */
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_EXCEPTION_WRAPPER_THROW_VOID(apiFunction, ...) \
  Result r;                                                 \
  apiFunction(__VA_ARGS__, &r);                             \
  if (r.failed())                                           \
    throw std::runtime_error(static_cast<char*>(r));

}  // namespace yaul

/******************************* Miscellaneous ********************************/

/**
 * @brief Decode an integer from a hex character representation
 * '0' to '9' is 0 to 9
 * 'a' to 'f' or 'A' to 'F' is 10 to 15
 * Other characters will throw invalid_argument
 *
 * @param c hex character to decode (0123456789abcdefABCDEF)
 * @return constexpr uint8_t value represented by the hex character
 */
YAUL_CONSTEXPR uint8_t decodeHexChar(const char c) noexcept(false) {
  if (c >= '0' && c <= '9') {
    return static_cast<uint8_t>(c - '0');
  }
  constexpr char aOffset = 10;
  if (c >= 'a' && c <= 'f') {
    return static_cast<uint8_t>(c - 'a' + aOffset);
  }
  if (c >= 'A' && c <= 'F') {
    return static_cast<uint8_t>(c - 'A' + aOffset);
  }
  throw std::invalid_argument(
      "invalid character encountered while decoding hex character");
}

#endif /* _YAUL_COMMON_HPP_ */