#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

HINSTANCE Window::Impl::hInstance = nullptr;

const LPCWSTR windowClassName = L"yaul";

void Window::Impl::adjustClientRect(HWND window, RECT& rect) noexcept {
  WINDOWPLACEMENT placement;
  if (::GetWindowPlacement(window, &placement) == 0 ||
      placement.showCmd != SW_MAXIMIZE) {
    return;
  }

  HMONITOR monitor = ::MonitorFromWindow(window, MONITOR_DEFAULTTONULL);
  if (monitor == nullptr) {
    return;
  }

  MONITORINFO monitorInfo{};
  monitorInfo.cbSize = sizeof(monitorInfo);
  if (::GetMonitorInfoW(monitor, &monitorInfo) == 0) {
    return;
  }

  // Client area fills the monitor working area (no taskbar)
  rect = monitorInfo.rcWork;
}

LRESULT Window::Impl::hitTest(POINT cursor) const noexcept {
  RECT window;
  if (::GetWindowRect(nativeWindow, &window) == 0) {
    return HTNOWHERE;
  }

  if (resizable) {
    enum EdgeMask : uint8_t {
      top    = 0x01,
      right  = 0x02,
      bottom = 0x04,
      left   = 0x08
    };
    // clang-format off
    auto edgeMask =
        (cursor.y < (window.top     + resizingBorder.top)     ? top     : 0) |
        (cursor.x > (window.right   - resizingBorder.right)   ? right   : 0) |
        (cursor.y > (window.bottom  - resizingBorder.bottom)  ? bottom  : 0) |
        (cursor.x < (window.left    + resizingBorder.left)    ? left    : 0);
    switch (edgeMask) {
      case top:             return HTTOP;
      case top | right:     return HTTOPRIGHT;
      case right:           return HTRIGHT;
      case bottom | right:  return HTBOTTOMRIGHT;
      case bottom:          return HTBOTTOM;
      case bottom | left:   return HTBOTTOMLEFT;
      case left:            return HTLEFT;
      case top | left:      return HTTOPLEFT;
    }
    // clang-format on
  }

  if (cursor.y < (window.top + draggingAreaBottom)) {
    return HTCAPTION;
  }

  return HTCLIENT;
}

LRESULT CALLBACK Window::Impl::windowProcedure(HWND hWindow,
                                               UINT msg,
                                               WPARAM wParam,
                                               LPARAM lParam) {
  auto* window = static_cast<Window::Impl*>(::GetPropW(hWindow, L"yaul"));
  if (window == nullptr) {
    return ::DefWindowProcW(hWindow, msg, wParam, lParam);
  }

  switch (msg) {
    case WM_NCCALCSIZE: {
      std::lock_guard<std::mutex> lock(window->mutex);

      // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
      auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
      if (wParam == TRUE && window->borderless) {
        // When computing client area for borderless, draw over title bar
        // A normal frame adjusts the client area to exclude the title bar
        adjustClientRect(hWindow, params.rgrc[0]);
        return 0;
      }
    } break;
    case WM_NCHITTEST: {
      std::lock_guard<std::mutex> lock(window->mutex);
      if (window->borderless && !window->fullscreen) {
        return window->hitTest(
            POINT{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
      }
    } break;
    case WM_NCACTIVATE: {
      std::lock_guard<std::mutex> lock(window->mutex);
      if (window->borderless && window->fullscreen) {
        // No activation changes when fullscreened
        // Without, it will have a basic title bar get painted upon deactivation
        return FALSE;
      }
      if (!compositionEnabled()) {
        // Prevents window frame reappearing on window activation
        // in "basic" theme, where no aero shadow is present.
        return TRUE;
      }
    } break;
    case WM_CREATE:
      return 0;
    case WM_CLOSE:
      window->closeRequest();
      return 0;
    case WM_MOVE:
      return 0;
    case WM_PAINT: {
      std::lock_guard<std::mutex> lock(window->mutex);
      PAINTSTRUCT ps;
      HDC hdc = nullptr;
      hdc     = BeginPaint(hWindow, &ps);

      Size size = window->getSize(false);
      string text =
          std::to_string(size.width) + "x" + std::to_string(size.height);
      TextOutA(hdc, 0, 15, text.c_str(), static_cast<int>(text.size()));
      size = window->getSize(true);
      text = std::to_string(size.width) + "x" + std::to_string(size.height);
      TextOutA(hdc, 0, 30, text.c_str(), static_cast<int>(text.size()));
      RECT rect;
      ::GetWindowRect(hWindow, &rect);
      text = std::to_string(rect.left) + " " + std::to_string(rect.top);
      TextOutA(hdc, 0, 45, text.c_str(), static_cast<int>(text.size()));
      EndPaint(hWindow, &ps);
      return 0;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: {
      switch (wParam) {
        case VK_F8: {
          window->setFullscreen(!window->fullscreen, 0, false);
          return 0;
        }
        case VK_F9: {
          if (!window->fullscreen) {
            window->setResizable(!window->resizable, false);
          }
          return 0;
        }
        case VK_F10: {
          if (!window->fullscreen) {
            window->setBorderless(!window->borderless, false);
          }
          return 0;
        }
        case VK_F11: {
          if (!window->fullscreen) {
            window->setBorderlessShadow(!window->borderlessShadow, false);
          }
          return 0;
        }
      }
      break;
    }
  }

  return ::DefWindowProcW(hWindow, msg, wParam, lParam);
}

void Window::Impl::createNativeWindow() noexcept(false) {
  if (hInstance == nullptr) {
    hInstance = ::GetModuleHandleW(nullptr);

    WNDCLASSEXW wc;
    ::ZeroMemory(&wc, sizeof(WNDCLASSEXW));

    wc.cbSize        = sizeof(wc);
    wc.lpszClassName = windowClassName;
    wc.lpfnWndProc   = windowProcedure;
    wc.hInstance     = hInstance;
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
    wc.hCursor = ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));
    if (::RegisterClassExW(&wc) == 0) {
      throw std::exception("Failed to register class");
    }
  }

  nativeWindow = ::CreateWindowExW(
      0, windowClassName, windowClassName,
      static_cast<DWORD>(WindowStyle::windowed), CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
  if (nativeWindow == nullptr) {
    throw std::exception("Failed to create window");
  }

  ::SetPropW(nativeWindow, L"yaul", this);
}

bool Window::Impl::compositionEnabled() noexcept {
  BOOL enabled = FALSE;
  bool success = ::DwmIsCompositionEnabled(&enabled) == S_OK;
  return (enabled == TRUE) && success;
}

}  // namespace yaul