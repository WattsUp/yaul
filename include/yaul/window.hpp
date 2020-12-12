#ifndef _YAUL_WINDOW_HPP_
#define _YAUL_WINDOW_HPP_

#include "common.hpp"

namespace yaul {

class YAUL_API Window {
 public:
  /**
   * @brief Construct a new Window object with a unique ID
   *
   * @param id of the window to refer to in XML/CSS
   */
  explicit Window(const char* id = "");
  ~Window();

  Window(Window&& o) noexcept;
  Window& operator=(Window&& o) noexcept;

  Window(const Window& o);
  Window& operator=(const Window& o);

 private:
  class Impl;
  Impl* pImpl;
};

}  // namespace yaul

#endif /* _YAUL_WINDOW_HPP_ */