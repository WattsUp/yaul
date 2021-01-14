#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

Window::Impl::Impl(Size size,
                   const char* title,
                   ShowState state) noexcept(false)
    : title(title), showState(state) {
  if (size.width == 0 || size.height == 0) {
    throw std::invalid_argument("Both dimensions of size must be non-zero");
  }
  createNativeWindow();
  setSize(size, true);
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
  if (innerSize && !borderless) {
    RECT rect{0, 0, size.width, size.height};
    auto style =
        static_cast<DWORD>(::GetWindowLongPtrW(nativeWindow, GWL_STYLE));
    if (::AdjustWindowRect(&rect, style, FALSE) == 0) {
      return false;
    }
    size = {rect.right - rect.left, rect.bottom - rect.top};
  }
  return ::SetWindowPos(nativeWindow, HWND_TOP, 0, 0, size.width, size.height,
                        SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER |
                            SWP_ASYNCWINDOWPOS) != 0;
#endif /* WIN32 */
}

Size Window::Impl::getSize(bool innerSize) const noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  RECT rect;
  if (borderless || innerSize) {
    ::GetClientRect(nativeWindow, &rect);
  } else {
    ::GetWindowRect(nativeWindow, &rect);
  }
  Size size = {rect.right - rect.left, rect.bottom - rect.top};
#endif /* WIN32 */
  return size;
}

bool Window::Impl::setPosition(Position position, int monitor) noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (monitor != 0) {
    // TODO (WattsUp)
    position.left -= 1920;
  }
  return ::SetWindowPos(
             nativeWindow, HWND_TOP, position.left, position.top, 0, 0,
             SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER | SWP_ASYNCWINDOWPOS) !=
         0;
#endif /* WIN32 */
}

void Window::Impl::setTitle(const char* title, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  this->title = string(title);
  if (lockMutex) {
    mutex.unlock();
  }
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  WideChar wTitle(title);
  ::SetWindowTextW(nativeWindow, wTitle.c_str());
#endif /* WIN32 */
}

void Window::Impl::setResizable(bool resizable, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  this->resizable = resizable;
  if (lockMutex) {
    mutex.unlock();
  }
}

void Window::Impl::setResizingBorder(Edges border, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  resizingBorder = border;
  if (lockMutex) {
    mutex.unlock();
  }
}

void Window::Impl::setBorderless(bool borderless, bool lockMutex) noexcept {
  if (this->borderless == borderless) {
    // No change
    return;
  }
  if (lockMutex) {
    mutex.lock();
  }
  this->borderless = borderless;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  Size innerSize = getSize(true);

  WindowStyle style = WindowStyle::windowed;
  if (borderless) {
    style = compositionEnabled() ? WindowStyle::aeroBorderless
                                 : WindowStyle::basicBorderless;
  }
  ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, static_cast<LONG>(style));

  // Restore shadow or not
  setBorderlessShadow(borderlessShadow, false);

  // Alert window manager frame has changed and preserve client area
  setSize(innerSize, true);
#endif /* WIN32 */

  setShowState(showState, false);
  if (lockMutex) {
    mutex.unlock();
  }
}

void Window::Impl::setBorderlessShadow(bool shadow, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  this->borderlessShadow = shadow;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (compositionEnabled()) {
    if (!borderless) {
      shadow = false;
    }
    // Note GDI cannot paint this margin away, use other graphics to swap frame
    // buffers
    if (shadow) {
      static const MARGINS margins{1, 1, 1, 1};
      ::DwmExtendFrameIntoClientArea(nativeWindow, &margins);
    } else {
      static const MARGINS margins{0, 0, 0, 0};
      ::DwmExtendFrameIntoClientArea(nativeWindow, &margins);
    }
  }
#endif /* WIN32 */

  if (lockMutex) {
    mutex.unlock();
  }
}

void Window::Impl::setDraggable(bool draggable, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  this->draggable = draggable;
  if (lockMutex) {
    mutex.unlock();
  }
}

void Window::Impl::setDraggingArea(int bottom, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  this->draggingAreaBottom = bottom;
  if (lockMutex) {
    mutex.unlock();
  }
}

void Window::Impl::setShowState(ShowState state, bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  showState = state;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  switch (state) {
    case ShowState::hidden:
      ShowWindowAsync(nativeWindow, SW_HIDE);
      break;
    case ShowState::minimize:
      ShowWindowAsync(nativeWindow, SW_SHOWMINIMIZED);
      break;
    case ShowState::maximize:
      ShowWindowAsync(nativeWindow, SW_SHOWMAXIMIZED);
      break;
    case ShowState::restore:
      ShowWindowAsync(nativeWindow, SW_RESTORE);
      break;
  }
#endif /* WIN32 */

  if (lockMutex) {
    mutex.unlock();
  }
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

void Window::setBorderlessShadow(bool shadow) noexcept {
  dynamic_cast<Window::Impl*>(pImpl)->setBorderlessShadow(shadow);
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