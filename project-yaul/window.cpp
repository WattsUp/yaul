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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  fullscreenPrePlacement.length = sizeof(WINDOWPLACEMENT);
#endif /* WIN32 */
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
  return ::SetWindowPos(nativeWindow, nullptr, 0, 0, size.width, size.height,
                        SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER |
                            SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS) != 0;
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
  return ::SetWindowPos(nativeWindow, nullptr, position.left, position.top, 0,
                        0,
                        SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER |
                            SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS) != 0;
#endif /* WIN32 */
}

void Window::Impl::setFullscreen(bool fullscreen,
                                 int /* monitor */,
                                 bool lockMutex) noexcept {
  if (lockMutex) {
    mutex.lock();
  }
  this->fullscreen = fullscreen;
  if (fullscreen) {
    // Save state information to restore to
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ::GetWindowPlacement(nativeWindow, &fullscreenPrePlacement);
    fullscreenPreStyle = ::GetWindowLongPtrW(nativeWindow, GWL_STYLE);

    auto style = fullscreenPreStyle & ~(WS_OVERLAPPEDWINDOW | WS_POPUP);
    ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, style);

    // TODO (WattsUp)
    HMONITOR monitor = ::MonitorFromWindow(nativeWindow, MONITOR_DEFAULTTONULL);
    if (monitor == nullptr) {
      if (lockMutex) {
        mutex.unlock();
      }
      return;
    }
    MONITORINFO monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    if (::GetMonitorInfoW(monitor, &monitorInfo) == 0) {
      if (lockMutex) {
        mutex.unlock();
      }
      return;
    }

    ::SetWindowPos(nativeWindow, HWND_TOP, monitorInfo.rcMonitor.left,
                   monitorInfo.rcMonitor.top,
                   monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
                   monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
                   SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOOWNERZORDER |
                       SWP_ASYNCWINDOWPOS);
#endif /* WIN32 */
  } else {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, fullscreenPreStyle);
    ::SetWindowPlacement(nativeWindow, &fullscreenPrePlacement);
    ::SetWindowPos(nativeWindow, nullptr, 0, 0, 0, 0,
                   SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);

#endif /* WIN32 */
  }
  if (lockMutex) {
    mutex.unlock();
  }
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

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (!borderless) {
    auto style = ::GetWindowLongPtrW(nativeWindow, GWL_STYLE);
    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    if (resizable) {
      style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
    }
    ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, style);
    ::SetWindowPos(nativeWindow, nullptr, 0, 0, 0, 0,
                   SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);
  }
#endif /* WIN32 */

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
  if (lockMutex) {
    mutex.lock();
  }
  this->borderless = borderless;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  Size innerSize = getSize(true);

  // Borderless related source from https://github.com/melak47/BorderlessWindow
  WindowStyle style = WindowStyle::windowed;
  if (borderless) {
    style = compositionEnabled() ? WindowStyle::aeroBorderless
                                 : WindowStyle::basicBorderless;
  }
  ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, static_cast<LONG_PTR>(style));

  // Restore shadow or not
  setBorderlessShadow(borderlessShadow, false);

  // Alert window manager frame has changed
  ::SetWindowPos(nativeWindow, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                     SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);

  // Preserve inner size when changing
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
                                Result* const r) noexcept {
  // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
  YAUL_EXCEPTION_WRAPPER_CATCH(return new Window(size, title, state));
  return nullptr;
}

bool Window::shouldClose() noexcept {
  return impl<Impl>()->shouldClose();
}

void Window::render() noexcept {
  impl<Impl>()->render();
}

void Window::pollEvents() noexcept {
  impl<Impl>()->pollEvents();
}

void Window::closeRequest() noexcept {
  impl<Impl>()->closeRequest();
}

bool Window::setSize(Size size, bool innerSize) noexcept {
  return impl<Impl>()->setSize(size, innerSize);
}

Size Window::getSize(bool innerSize) const noexcept {
  return impl<Impl>()->getSize(innerSize);
}

bool Window::setPosition(Position position, int monitor) noexcept {
  return impl<Impl>()->setPosition(position, monitor);
}

void Window::setFullscreen(bool fullscreen, int monitor) noexcept {
  impl<Impl>()->setFullscreen(fullscreen, monitor);
}

void Window::setTitle(const char* title) noexcept {
  impl<Impl>()->setTitle(title);
}

void Window::setResizable(bool resizable) noexcept {
  impl<Impl>()->setResizable(resizable);
}

void Window::setResizingBorder(Edges border) noexcept {
  impl<Impl>()->setResizingBorder(border);
}

void Window::setBorderless(bool borderless) noexcept {
  impl<Impl>()->setBorderless(borderless);
}

void Window::setBorderlessShadow(bool shadow) noexcept {
  impl<Impl>()->setBorderlessShadow(shadow);
}

void Window::setDraggingArea(int bottom) noexcept {
  impl<Impl>()->setDraggingArea(bottom);
}

void Window::setShowState(ShowState state) noexcept {
  impl<Impl>()->setShowState(state);
}

}  // namespace yaul