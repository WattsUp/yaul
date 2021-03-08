#ifndef YAUL_STRING_HPP
#define YAUL_STRING_HPP

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4127)  // Conditional expression is constant
#endif

#include <tinyutf8.h>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <memory>
#include <yaul/common.hpp>

namespace yaul {

using string = tiny_utf8::utf8_string;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace yaul {

class WideChar final {
 public:
  /**
   * @brief Construct a new wide char string from a UTF8 string
   *
   * @param string to convert, UTF-8 encoded
   */
  explicit WideChar(const string& string) noexcept
      : len(::MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, nullptr, 0)),
        // NOLINTNEXTLINE (modernize-avoid-c-arrays)
        buffer(std::make_unique<wchar_t[]>(static_cast<size_t>(len))) {
    ::MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, buffer.get(), len);
  }

  /**
   * @brief Get the string as a null terminated wide char array
   *
   * @return wchar_t*
   */
  [[nodiscard]] inline const wchar_t* c_str() const noexcept {
    return buffer.get();
  }

  /**
   * @brief Convert a wide char string to a UTF8 string
   *
   * @param wString to convert
   * @return string UTF-8 encoded
   */
  static inline string rev(const wchar_t* wString) noexcept {
    int len = WideCharToMultiByte(CP_UTF8, 0, wString, -1, nullptr, 0, nullptr,
                                  nullptr);
    std::unique_ptr<char[]> buffer(  // NOLINT (modernize-avoid-c-arrays)
        std::make_unique<char[]>(    // NOLINT (modernize-avoid-c-arrays)
            static_cast<size_t>(len)));
    WideCharToMultiByte(CP_UTF8, 0, wString, -1, buffer.get(), len, nullptr,
                        nullptr);
    return string(buffer.get());
  }

 private:
  int len = 0;
  std::unique_ptr<wchar_t[]> buffer;  // NOLINT (modernize-avoid-c-arrays)
};

}  // namespace yaul
#endif /* WIN32 */

#endif /* YAUL_STRING_HPP */