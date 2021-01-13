#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

Window::Impl::Impl(Size size,
                   const char* title,
                   ShowState state) noexcept(false)
    : title(title) {
  if (size.width == 0 || size.height == 0) {
    throw std::invalid_argument("Both dimensions of size must be non-zero");
  }
  createNativeWindow(size);
  setTitle(title);
  setShowState(state);
}

Window::Impl::~Impl() noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  ::RemovePropW(nativeWindow, L"yaul");
  ::DestroyWindow(nativeWindow);
#endif /* WIN32 */
}

bool Window::Impl::shouldClose() const noexcept {
  return closeFlag;
}

void Window::Impl::render() noexcept {
  // TODO (WattsUp)
}

void Window::Impl::pollEvents() noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  MSG msg;
  while (::PeekMessageW(&msg, nativeWindow, 0, 0, PM_REMOVE) != 0) {
    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);
  }
#endif /* WIN32 */
}

bool Window::Impl::setSize(Size size, bool innerSize) noexcept {
  if (size.width == 0 || size.height == 0) {
    return false;
  }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (innerSize) {
    // TODO (WattsUp)
    size.width += 10;
    size.height += 10;
  }
  return ::SetWindowPos(nativeWindow, HWND_TOP, 0, 0, size.width, size.height,
                        SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER) != 0;
#endif /* WIN32 */
}

Size Window::Impl::getSize(bool innerSize) const noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  RECT rect;
  ::GetWindowRect(nativeWindow, &rect);
  Size size = {rect.right - rect.left, rect.bottom - rect.top};
  if (innerSize) {
    // TODO (WattsUp)
    size.width -= 10;
    size.height -= 10;
  }
  return size;
#endif /* WIN32 */
}

bool Window::Impl::setPosition(Position position, int monitor) noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (monitor != 0) {
    // TODO (WattsUp)
    position.left -= 1920;
  }
  return ::SetWindowPos(nativeWindow, HWND_TOP, position.left, position.top, 0,
                        0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER) != 0;
#endif /* WIN32 */
}

void Window::Impl::setTitle(const char* title) noexcept {
  this->title = string(title);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  WideChar wTitle(title);
  ::SetWindowTextW(nativeWindow, wTitle.c_str());
#endif /* WIN32 */
}

void Window::Impl::setResizable(bool resizable) noexcept {
  this->resizable = resizable;
}

void Window::Impl::setResizingBorder(Edges border) noexcept {
  resizingBorder = border;
}

void Window::Impl::setBorderless(bool borderless) noexcept {
  this->borderless = borderless;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#endif /* WIN32 */
}

void Window::Impl::setDraggable(bool draggable) noexcept {
  this->draggable = draggable;
}

void Window::Impl::setDraggingArea(int bottom) noexcept {
  this->draggingAreaBottom = bottom;
}

void Window::Impl::setShowState(ShowState state) noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  switch (state) {
    case ShowState::hidden:
      ShowWindow(nativeWindow, SW_HIDE);
      break;
    case ShowState::minimize:
      ShowWindow(nativeWindow, SW_SHOWMINIMIZED);
      break;
    case ShowState::maximize:
      ShowWindow(nativeWindow, SW_SHOWMAXIMIZED);
      break;
    case ShowState::restore:
      ShowWindow(nativeWindow, SW_RESTORE);
      break;
  }
#endif /* WIN32 */
}

/******************************** Public Class ********************************/

Window::Window(Size size, const char* title, ShowState state) noexcept(false)
    : Object(*new Window::Impl(size, title, state)) {}

YAUL_IMPL_DESTRUCT(Window);
YAUL_IMPL_MOVE(Window, Object);

Window* Window::apiCreateWindow(Size size,
                                const char* title,
                                ShowState state,
                                Result& r) noexcept {
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  YAUL_EXCEPTION_WRAPPER_CATCH(return new Window(size, title, state), r);
  return nullptr;
}

bool Window::shouldClose() noexcept {
  return dynamic_cast<Window::Impl*>(pImpl)->shouldClose();
}

void Window::render() noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->render();
}

void Window::pollEvents() noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->pollEvents();
}

void Window::closeRequest() noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->closeRequest();
}

bool Window::setSize(Size size, bool innerSize) noexcept {
  return dynamic_cast<Window::Impl*>(pImpl)->setSize(size, innerSize);
}

Size Window::getSize(bool innerSize) const noexcept {
  return dynamic_cast<Window::Impl*>(pImpl)->getSize(innerSize);
}

bool Window::setPosition(Position position, int monitor) noexcept {
  return dynamic_cast<Window::Impl*>(pImpl)->setPosition(position, monitor);
}

void Window::setTitle(const char* title) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setTitle(title);
}

void Window::setResizable(bool resizable) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setResizable(resizable);
}

void Window::setResizingBorder(Edges border) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setResizingBorder(border);
}

void Window::setBorderless(bool borderless) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setBorderless(borderless);
}

void Window::setDraggable(bool draggable) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setDraggable(draggable);
}

void Window::setDraggingArea(int bottom) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setDraggingArea(bottom);
}

void Window::setShowState(ShowState state) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setShowState(state);
}

}  // namespace yaul