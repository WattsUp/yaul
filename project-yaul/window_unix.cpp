#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

void Window::Impl::createNativeWindow() noexcept(false) {
  if (xcb == nullptr)
    throw std::runtime_error("Failed to connect x server");

  // nativeWindow = xcb_generate_id(xcb->connection);
}

}  // namespace yaul