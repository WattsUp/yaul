#include "yaul/window.hpp"

#include "logger.hpp"

namespace yaul {

class Window::Impl {
 public:
  explicit Impl(const char* id);
};

/**
 * @brief Construct a new Window::Impl object
 * Implementation for pimpl design
 *
 */
Window::Impl::Impl(const char* id) {
  Logger::instance().log(LogLevel::debug, "New window");
  Logger::instance().log(LogLevel::debug, id);
}

/******************************* pimpl Wrapper ********************************/

Window::Window(const char* id) : pImpl(new Impl(id)) {}

Window::~Window() {
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

Window::Window(const Window& o) : pImpl(new Impl(*o.pImpl)) {}

Window& Window::operator=(const Window& o) {
  if (this != &o) {
    pImpl = new Impl(*o.pImpl);  // NOLINT (cppcoreguidelines-owning-memory)
  }
  return *this;
}

}  // namespace yaul