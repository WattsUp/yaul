#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

Window::Impl::Impl(Size size,
                   const char* title,
                   ShowState state) noexcept(false)
    : title(title), showState(state) {
  if (size.width == 0 || size.height == 0)
    throw std::invalid_argument("Both dimensions of size must be non-zero");

  createNativeWindow();
  setSize(size);
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
#elif defined(__linux) || defined(__linux__)
  try {
    windowIDs.erase(nativeWindow);

    const auto* xcb = XCB::instance();
    xcb_free_colormap(xcb->connection(), colormap);
    xcb_destroy_window(xcb->connection(), nativeWindow);
    xcb_flush(xcb->connection());
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
#endif /* WIN32, __linux__ */
}

bool Window::Impl::shouldClose() const noexcept {
  return closeFlag;
}

void Window::Impl::render() noexcept {
  // TODO (WattsUp)
}

bool Window::Impl::setSize(Size size) noexcept {
  if (size.width == 0 || size.height == 0)
    return false;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (!borderless) {
    RECT rect{0, 0, size.width, size.height};
    auto style =
        static_cast<DWORD>(::GetWindowLongPtrW(nativeWindow, GWL_STYLE));
    if (::AdjustWindowRect(&rect, style, FALSE) == 0)
      return false;

    size = {rect.right - rect.left, rect.bottom - rect.top};
  }
  return ::SetWindowPos(nativeWindow, nullptr, 0, 0, size.width, size.height,
                        SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER |
                            SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS) != 0;

#elif defined(__linux) || defined(__linux__)
  try {
    const auto* xcb = XCB::instance();

    const uint16_t mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
    const std::array<uint32_t, 2> values = {static_cast<uint32_t>(size.width),
                                            static_cast<uint32_t>(size.height)};
    xcb_configure_window(xcb->connection(), nativeWindow, mask, values.data());
    frameMarginsDirty = true;

    xcb_flush(xcb->connection());
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
    return false;
  }
  return true;
#endif /* WIN32, __linux__ */
}

Size Window::Impl::getSize() const noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  RECT rect;
  ::GetClientRect(nativeWindow, &rect);

  return Size{rect.right - rect.left, rect.bottom - rect.top};
#elif defined(__linux) || defined(__linux__)
  return lastSizeUpdate;
#endif /* WIN32, __linux__ */
}

bool Window::Impl::setPosition(Position position,
                               const Monitor* monitor) noexcept {
  if (monitor != nullptr)
    position += monitor->getPosition();
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

  return ::SetWindowPos(nativeWindow, nullptr, position.x, position.y, 0, 0,
                        SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER |
                            SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS) != 0;

#elif defined(__linux) || defined(__linux__)
  position -= updateFrameMargins();

  try {
    const auto* xcb = XCB::instance();

    const uint16_t mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
    const std::array<uint32_t, 2> values = {static_cast<uint32_t>(position.x),
                                            static_cast<uint32_t>(position.y)};
    xcb_configure_window(xcb->connection(), nativeWindow, mask, values.data());

    xcb_flush(xcb->connection());
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
    return false;
  }
  return true;
#endif /* WIN32, __linux__ */
}

void Window::Impl::setFullscreen(bool fullscreen,
                                 const Monitor* monitor,
                                 bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setFullscreen(fullscreen, monitor, false);
  }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  bool overwriteSave = (!this->fullscreen) && fullscreen;
  this->fullscreen   = fullscreen;
  if (fullscreen) {
    // Save state information to restore to but do not overwrite if already
    // fullscreen
    if (overwriteSave) {
      ::GetWindowPlacement(nativeWindow, &fullscreenPrePlacement);
      fullscreenPreStyle = ::GetWindowLongPtrW(nativeWindow, GWL_STYLE);
    }

    auto style = fullscreenPreStyle & ~(WS_OVERLAPPEDWINDOW | WS_POPUP);
    ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, style);

    Size size;
    Position position;
    if (monitor == nullptr) {
      // Fullscreen current monitor
      HMONITOR monitor =
          ::MonitorFromWindow(nativeWindow, MONITOR_DEFAULTTONULL);
      if (monitor == nullptr)
        return;

      MONITORINFO monitorInfo{};
      monitorInfo.cbSize = sizeof(monitorInfo);
      if (::GetMonitorInfoW(monitor, &monitorInfo) == 0)
        return;

      size.width  = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
      size.height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
      position.x  = monitorInfo.rcMonitor.left;
      position.y  = monitorInfo.rcMonitor.top;
    } else {
      size     = monitor->getSize();
      position = monitor->getPosition();
    }

    ::SetWindowPos(nativeWindow, HWND_TOP, position.x, position.y, size.width,
                   size.height,
                   SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOOWNERZORDER |
                       SWP_ASYNCWINDOWPOS);
  } else {
    ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, fullscreenPreStyle);
    ::SetWindowPlacement(nativeWindow, &fullscreenPrePlacement);
    ::SetWindowPos(nativeWindow, nullptr, 0, 0, 0, 0,
                   SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);
  }
#elif defined(__linux) || defined(__linux__)
  this->fullscreen = fullscreen;
  try {
    XCB::instance()->setWMState(nativeWindow, fullscreen,
                                XCB::WMState::fullscreen);
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
#endif /* WIN32, __linux__ */
}

void Window::Impl::setTitle(const char* title, bool lockMutex) noexcept {
  if (lockMutex)
    mutex.lock();

  this->title = string(title);

  if (lockMutex)
    mutex.unlock();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  WideChar wTitle(this->title);
  ::SetWindowTextW(nativeWindow,
                   wTitle.c_str());  // Not asynchronous, unlock mutex before
#elif defined(__linux) || defined(__linux__)
  try {
    const auto* xcb = XCB::instance();

    xcb_change_property(xcb->connection(), XCB_PROP_MODE_REPLACE, nativeWindow,
                        xcb->atom(XCB::NET_WM_NAME),
                        xcb->atom(XCB::UTF8_STRING), 8,
                        static_cast<uint32_t>(strlen(title)), title);

    xcb_flush(xcb->connection());
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
#endif /* WIN32, __linux__ */
}

void Window::Impl::setResizable(bool resizable, bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setResizable(resizable, false);
  }

  this->resizable = resizable;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (!borderless) {
    auto style = ::GetWindowLongPtrW(nativeWindow, GWL_STYLE);
    style &= ~(WS_SIZEBOX | WS_MAXIMIZEBOX);
    if (resizable)
      style |= WS_SIZEBOX | WS_MAXIMIZEBOX;

    ::SetWindowLongPtrW(nativeWindow, GWL_STYLE, style);
    ::SetWindowPos(nativeWindow, nullptr, 0, 0, 0, 0,
                   SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);
  }
#elif defined(__linux) || defined(__linux__)
  try {
    XCB::instance()->setWMHints(nativeWindow, !borderless, resizable);
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
#endif /* WIN32, __linux__ */
}

void Window::Impl::setResizingBorder(Edges border, bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setResizingBorder(border, false);
  }

  resizingBorder = border;
}

void Window::Impl::setBorderless(bool borderless, bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setBorderless(borderless, false);
  }

  this->borderless = borderless;
  Size innerSize   = getSize();

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  // Borderless related source from
  // https://github.com/melak47/BorderlessWindow
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

#elif defined(__linux) || defined(__linux__)
  try {
    XCB::instance()->setWMHints(nativeWindow, !borderless, resizable);
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
#endif /* WIN32, __linux__ */
  // Preserve inner size when changing
  setSize(innerSize);

  setShowState(showState, false);
}

void Window::Impl::setBorderlessShadow(bool shadow, bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setBorderlessShadow(shadow, false);
  }

  this->borderlessShadow = shadow;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  if (compositionEnabled()) {
    if (!borderless)
      shadow = false;

    // Note GDI cannot paint this margin away, use other graphics to swap
    // frame buffers
    if (shadow) {
      static const MARGINS margins{1, 1, 1, 1};
      ::DwmExtendFrameIntoClientArea(nativeWindow, &margins);
    } else {
      static const MARGINS margins{0, 0, 0, 0};
      ::DwmExtendFrameIntoClientArea(nativeWindow, &margins);
    }
  }
#elif defined(__linux) || defined(__linux__)
  // TODO (WattsUp), impossible?
#endif /* WIN32, __linux__ */
}

void Window::Impl::setDraggingArea(int bottom,
                                   int menuWidth,
                                   bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setDraggingArea(bottom, menuWidth, false);
  }

  this->draggingAreaBottom = bottom;
  this->menuWidth          = menuWidth;
}

void Window::Impl::clearDraggingAreaMasks(bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return clearDraggingAreaMasks(false);
  }

  draggingAreaMasks.clear();
}

void Window::Impl::addDraggingAreaMask(Rectangle rect,
                                       bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return addDraggingAreaMask(rect, false);
  }

  draggingAreaMasks.emplace_back(rect);
}

void Window::Impl::setShowState(ShowState state, bool lockMutex) noexcept {
  if (lockMutex) {
    std::lock_guard<std::mutex> lock(mutex);
    return setShowState(state, false);
  }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  showState = state;
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
#elif defined(__linux) || defined(__linux__)
  if (showState == state)
    return;

  try {
    const auto* xcb = XCB::instance();

    if (state == ShowState::hidden) {
      // Unmap window and notify
      xcb_unmap_window(xcb->connection(), nativeWindow);

      xcb_unmap_notify_event_t event;
      event.response_type  = XCB_UNMAP_NOTIFY;
      event.event          = xcb->screen()->root;
      event.window         = nativeWindow;
      event.from_configure = False;
      YAUL_XCB_SEND(xcb, xcb->screen()->root,
                    XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
                    event);
      xcb_flush(xcb->connection());
      showState = state;
      mapped    = false;
      return;
    }
    if (showState == ShowState::minimize || !mapped) {
      xcb_map_window(xcb->connection(), nativeWindow);
      mapped = true;
    } else if (state == ShowState::minimize) {
      // Send ICONIC state
      xcb_client_message_event_t event;
      event.response_type  = XCB_CLIENT_MESSAGE;
      event.format         = 32;
      event.sequence       = 0;
      event.window         = nativeWindow;
      event.type           = xcb->atom(XCB::WM_CHANGE_STATE);
      event.data.data32[0] = XCB_ICCCM_WM_STATE_ICONIC;
      event.data.data32[1] = 0;
      event.data.data32[2] = 0;
      event.data.data32[3] = 0;
      event.data.data32[4] = 0;
      YAUL_XCB_SEND(xcb, xcb->screen()->root,
                    XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
                    event);
    } else if (state == ShowState::maximize) {
      xcb->setWMState(nativeWindow, true, XCB::WMState::maximize);
    }

    auto cookie =
        xcb_icccm_get_wm_hints_unchecked(xcb->connection(), nativeWindow);
    xcb_icccm_wm_hints_t hints;
    if (xcb_icccm_get_wm_hints_reply(xcb->connection(), cookie, &hints,
                                     nullptr) != 0) {
      if (state == ShowState::minimize) {
        xcb_icccm_wm_hints_set_iconic(&hints);
      } else {
        xcb_icccm_wm_hints_set_normal(&hints);
      }
      xcb_icccm_set_wm_hints(xcb->connection(), nativeWindow, &hints);
    }

    xcb_flush(xcb->connection());
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
  showState = state;
#endif /* WIN32, __linux__ */
}

/******************************** Public Class
 * ********************************/

Window::Window() noexcept : SharedObject(std::shared_ptr<Impl>(nullptr)) {}

YAUL_IMPL_DESTRUCT(Window);
YAUL_IMPL_MOVE(Window, SharedObject);
YAUL_IMPL_COPY_SHARED(Window);

void Window::apiCreateWindow(Size size,
                             const char* title,
                             ShowState state,
                             Result* const r) noexcept {
  try {
    SharedObject::setImpl(std::make_shared<Window::Impl>(size, title, state));
  } catch (const std::exception& e) {
    *(r) = Result(e.what());
  }
}

bool Window::shouldClose() noexcept {
  return impl<Impl>()->shouldClose();
}

void Window::render() noexcept {
  impl<Impl>()->render();
}

void Window::closeRequest() noexcept {
  impl<Impl>()->closeRequest();
}

bool Window::setSize(Size size) noexcept {
  return impl<Impl>()->setSize(size);
}

Size Window::getSize() const noexcept {
  return impl<Impl>()->getSize();
}

bool Window::setPosition(Position position, const Monitor* monitor) noexcept {
  return impl<Impl>()->setPosition(position, monitor);
}

void Window::setFullscreen(bool fullscreen, const Monitor* monitor) noexcept {
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

void Window::setDraggingArea(int bottom, int menuWidth) noexcept {
  impl<Impl>()->setDraggingArea(bottom, menuWidth);
}

void Window::clearDraggingAreaMasks() noexcept {
  impl<Impl>()->clearDraggingAreaMasks();
}

void Window::addDraggingAreaMask(Rectangle rect) noexcept {
  impl<Impl>()->addDraggingAreaMask(rect);
}

void Window::setShowState(ShowState state) noexcept {
  impl<Impl>()->setShowState(state);
}

Window::ShowState Window::getShowState() const noexcept {
  return impl<Impl>()->getShowState();
}

}  // namespace yaul