#ifndef _YAUL_WINDOW_HPP_
#define _YAUL_WINDOW_HPP_

#include "common.hpp"

namespace yaul {

namespace impl {
class Window;
}

class YAUL_API Window {
 public:
  explicit Window(const char* id = "") noexcept;
  ~Window() noexcept;

  Window(Window&& o) noexcept;
  Window& operator=(Window&& o) noexcept;

  Window(const Window& o) noexcept;
  Window& operator=(const Window& o) noexcept;

  /**
   * @brief Show this window (or hide)
   * No effect if already visible (or hidden)
   *
   * @param visible true will show this window, false will hide
   */
  void show(bool visible = true) noexcept;

 private:
  impl::Window* pImpl;
};

}  // namespace yaul

#endif /* _YAUL_WINDOW_HPP_ */