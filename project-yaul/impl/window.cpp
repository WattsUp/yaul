#include <yaul/window.hpp>

#include "logger.hpp"
#include "window.hpp"

namespace yaul::impl {

Window::Window(const char* id) noexcept {
  createWindow(string(id));
}

void Window::show(bool visible) {
#ifdef WIN32
  ::ShowWindow(window, visible ? SW_SHOW : SW_HIDE);
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#endif /* WIN32 */
}

}  // namespace yaul::impl