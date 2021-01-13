#ifndef _YAUL_STRING_HPP_
#define _YAUL_STRING_HPP_

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4127)  // Conditional expression is constant
#endif

#include <tinyutf8.h>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

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
  WideChar(string string) {
    int len = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, nullptr, 0);
    buffer  = new wchar_t[static_cast<size_t>(
        len)];  // NOLINT (cppcoreguidelines-owning-memory)
    MultiByteToWideChar(CP_UTF8, 0, string.c_str(), -1, buffer, len);
  }

  ~WideChar() { delete[] buffer; }

  YAUL_NO_COPY(WideChar);
  YAUL_NO_MOVE(WideChar);

  /**
   * @brief Get the string as a null terminated wide char array
   *
   * @return wchar_t*
   */
  wchar_t* c_str() const { return buffer; }

 private:
  wchar_t* buffer = nullptr;
};

}  // namespace yaul
#endif /* WIN32 */

#endif /* _YAUL_STRING_HPP_ */