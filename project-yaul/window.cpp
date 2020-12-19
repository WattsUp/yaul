#include <yaul/window.hpp>

#include "impl/window.hpp"
#include "logger.hpp"

namespace yaul {

Window::Window(const char* id) noexcept : pImpl(new impl::Window(id)) {}

Window::~Window() noexcept {
  delete pImpl;
}

Window::Window(Window&& o) noexcept {
  pImpl   = o.pImpl;
  o.pImpl = nullptr;
}

Window& Window::operator=(Window&& o) noexcept {
  if (&o == this) {
    return *this;
  }
  delete pImpl;
  pImpl   = o.pImpl;
  o.pImpl = nullptr;
  return *this;
}

Window::Window(const Window& o) noexcept : pImpl(new impl::Window(*o.pImpl)) {}

Window& Window::operator=(const Window& o) noexcept {
  if (this != &o) {
    // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
    pImpl = new impl::Window(*o.pImpl);
  }
  return *this;
}

void Window::show(bool visible) noexcept {
  pImpl->show(visible);
}

}  // namespace yaul