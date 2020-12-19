#ifndef _YAUL_IMPL_WINDOW_HPP_
#define _YAUL_IMPL_WINDOW_HPP_

#include <yaul/window.hpp>

#include "string.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#endif /* WIN32 */

namespace yaul::impl {

class Window {
 public:
  /**
   * @brief Construct a new Window::Impl object
   * Implementation for pimpl design
   *
   * @param id of the window to refer to in XML/CSS
   */
  explicit Window(const char* id) noexcept;

  void show(bool visible = true);

 private:
  void createWindow(const string& id);

#ifdef WIN32
  using NativeWindow_t = HWND;
#endif /* WIN32 */

  NativeWindow_t window = nullptr;
};

}  // namespace yaul::impl

#endif /* _YAUL_IMPL_WINDOW_HPP_ */