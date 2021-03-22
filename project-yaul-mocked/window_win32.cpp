#include "window_mocked.hpp"

#include <gtest/gtest.h>

int GetSystemMetrics_mocked(int nIndex) {
  switch (nIndex) {
    case SM_CYFRAME:
      return SystemMetricsMocked::cYFrame;
    case SM_CXFRAME:
      return SystemMetricsMocked::cXFrame;
    case SM_CXPADDEDBORDER:
      return SystemMetricsMocked::cXPaddedBorder;
    case SM_CYSIZE:
      return SystemMetricsMocked::cYSize;
    case SM_CXSIZE:
      return SystemMetricsMocked::cXSize;
  }
  return 0;
}

BOOL EnableMenuItem_mocked(HMENU /* hMenu */,
                           UINT /* uIDEnableItem */,
                           UINT /* uEnable */) {
  return FALSE;  // TODO (WattsUp) implement EnableMenuItem
}

HMODULE GetModuleHandleW_mocked(LPCWSTR lpModuleName) {
  if (lpModuleName == nullptr)
    return WindowManager::instance().handles.hModule;
  return nullptr;
}

HCURSOR LoadCursorW_mocked(HINSTANCE hInstance, LPCWSTR lpCursorName) {
  if (hInstance != nullptr)
    return nullptr;
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  if (lpCursorName == IDC_ARROW)
    return WindowManager::instance().handles.arrow;

  return nullptr;
}

ATOM RegisterClassExW_mocked(const WNDCLASSEXW* wc) {
  if (wc == nullptr)
    return 0;

  if (wc->cbSize != sizeof(WNDCLASSEXW))
    return 0;

  auto& wm            = WindowManager::instance();
  wm.windowClass.atom = static_cast<ATOM>(rand());

  wm.windowClass.className = ::yaul::WideChar::rev(wc->lpszClassName);

  wm.windowClass.wndProc = wc->lpfnWndProc;

  // TODO (WattsUp) add window class members

  return wm.windowClass.atom;
}

HWND CreateWindowExW_mocked(DWORD dwExStyle,
                            LPCWSTR lpClassName,
                            LPCWSTR lpWindowName,
                            DWORD dwStyle,
                            int X,
                            int Y,
                            int nWidth,
                            int nHeight,
                            HWND hWndParent,
                            HMENU hMenu,
                            HINSTANCE hInstance,
                            LPVOID lpParam) {
  auto& wm = WindowManager::instance();
  if (wm.windowClass.className != ::yaul::WideChar::rev(lpClassName))
    return nullptr;

  if (wm.handles.hModule != hInstance)
    return nullptr;

  if (hWndParent != nullptr || hMenu != nullptr || lpParam != nullptr)
    return nullptr;

  if (wm.windowClass.threadID == std::thread::id()) {
    wm.windowClass.threadID = std::this_thread::get_id();
  } else if (wm.windowClass.threadID != std::this_thread::get_id()) {
    return nullptr;
  }

  WindowManager::Window window;
  do {
    window.nativeHandle =
        // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<HWND>(::yaul::randRange(1, RAND_MAX));
  } while (wm.windows.find(window.nativeHandle) != wm.windows.end());

  window.style   = dwStyle;
  window.exStyle = dwExStyle;
  window.title   = ::yaul::WideChar::rev(lpWindowName);

  if (X == CW_USEDEFAULT) {
    window.frameRect.x = 32;
    window.frameRect.y = 32;

  } else {
    window.frameRect.x = X;
    window.frameRect.y = Y;
  }

  if (nWidth == CW_USEDEFAULT) {
    window.frameRect.width =
        ::yaul::randRange(SystemMetricsMocked::cXMin, 2048);
    window.frameRect.height =
        ::yaul::randRange(SystemMetricsMocked::cYMin, 2048);
  } else {
    window.frameRect.width  = nWidth;
    window.frameRect.height = nHeight;
  }

  wm.windows.emplace(window.nativeHandle, window);

  SetWindowPos(
      window.nativeHandle, nullptr, window.frameRect.x, window.frameRect.y,
      window.frameRect.width, window.frameRect.height,
      SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);

  if (X == CW_USEDEFAULT) {
    ::ShowWindow(window.nativeHandle, (Y == CW_USEDEFAULT) ? SW_SHOW : Y);
  }

  return window.nativeHandle;
}

BOOL DestroyWindow_mocked(HWND hWnd) {
  if (hWnd == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  if (wm.windowClass.threadID == std::thread::id()) {
    wm.windowClass.threadID = std::this_thread::get_id();
  } else if (wm.windowClass.threadID != std::this_thread::get_id()) {
    return FALSE;
  }

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  // auto& window = itr->second;

  wm.windowClass.wndProc(hWnd, WM_DESTROY, 0, 0);
  wm.windowClass.wndProc(hWnd, WM_NCDESTROY, 0, 0);

  // TODO (WattsUp) Destroy children

  wm.windows.erase(hWnd);
  return TRUE;
}

BOOL SetPropW_mocked(HWND hWnd, LPCWSTR lpString, HANDLE hData) {
  if (hWnd == nullptr || lpString == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  auto key = ::yaul::WideChar::rev(lpString);
  window.properties.erase(key);
  window.properties.emplace(key, hData);

  return TRUE;
}

HANDLE GetPropW_mocked(HWND hWnd, LPCWSTR lpString) {
  if (hWnd == nullptr || lpString == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  auto key     = ::yaul::WideChar::rev(lpString);
  auto propItr = window.properties.find(key);
  if (propItr == window.properties.end())
    return nullptr;

  return propItr->second;
}

HANDLE RemovePropW_mocked(HWND hWnd, LPCWSTR lpString) {
  if (hWnd == nullptr || lpString == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  auto key     = ::yaul::WideChar::rev(lpString);
  auto propItr = window.properties.find(key);
  if (propItr == window.properties.end())
    return nullptr;

  auto* data = propItr->second;

  window.properties.erase(key);

  return data;
}

LONG_PTR SetWindowLongPtrW_mocked(HWND hWnd, int nIndex, LONG_PTR dwNewLong) {
  if (hWnd == nullptr)
    return 0;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return 0;
  auto& window = itr->second;

  LONG_PTR prev = 0;
  switch (nIndex) {
    case GWL_EXSTYLE:
      prev           = window.exStyle;
      window.exStyle = static_cast<DWORD>(dwNewLong);
      break;
    case GWL_STYLE:
      prev         = window.style;
      window.style = static_cast<DWORD>(dwNewLong);
      break;
  }
  return prev;
}

LONG_PTR GetWindowLongPtrW_mocked(HWND hWnd, int nIndex) {
  if (hWnd == nullptr)
    return 0;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return 0;
  auto& window = itr->second;

  switch (nIndex) {
    case GWL_EXSTYLE:
      return window.exStyle;
    case GWL_STYLE:
      return window.style;
  }
  return 0;
}

BOOL AdjustWindowRect_mocked(LPRECT lpRect, DWORD dwStyle, BOOL bMenu) {
  if (lpRect == nullptr || bMenu != FALSE)
    return FALSE;

  auto edges = WindowManager::getEdges(dwStyle);
  lpRect->top -= edges.top;
  lpRect->right += edges.right;
  lpRect->bottom += edges.bottom;
  lpRect->left -= edges.left;

  return TRUE;
}

BOOL SetWindowPos_mocked(HWND hWnd,
                         HWND hWndInsertAfter,
                         int X,
                         int Y,
                         int cx,
                         int cy,
                         UINT uFlags) {
  if (hWnd == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  if ((uFlags & SWP_ASYNCWINDOWPOS) == SWP_ASYNCWINDOWPOS) {
    wm.asyncQueue.emplace_back([hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags] {
      UINT flags = uFlags & static_cast<UINT>(~SWP_ASYNCWINDOWPOS);
      SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, flags);
    });
    return TRUE;
  }

  // TODO (WattsUp) use hWndInsertAfter
  if ((uFlags & SWP_NOMOVE) == 0) {
    window.frameRect.x = X;
    window.frameRect.y = Y;
    window.restoreRect = window.frameRect;
  }
  if ((uFlags & SWP_NOSIZE) == 0) {
    window.frameRect.width  = cx;
    window.frameRect.height = cy;
    window.restoreRect      = window.frameRect;
  }

  if ((uFlags & SWP_FRAMECHANGED) == SWP_FRAMECHANGED ||
      (uFlags & SWP_NOSIZE) == 0) {
    calculateClientArea(hWnd);

    // Send WM_SIZE
  }

  return TRUE;
}

void calculateClientArea(HWND hWnd) {
  if (hWnd == nullptr)
    return;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return;
  auto& window = itr->second;

  NCCALCSIZE_PARAMS params;
  WINDOWPOS windowPos;
  params.lppos = &windowPos;
  // Not populating lppos

  // Proposed dimensions
  params.rgrc[0].left   = window.frameRect.x;
  params.rgrc[0].top    = window.frameRect.y;
  params.rgrc[0].right  = window.frameRect.x + window.frameRect.width;
  params.rgrc[0].bottom = window.frameRect.y + window.frameRect.height;

  // Previous dimensions
  params.rgrc[1].left   = 0;
  params.rgrc[1].top    = 0;
  params.rgrc[1].right  = 0;
  params.rgrc[1].bottom = 0;

  // Previous client area dimensions
  params.rgrc[2].left   = 0;
  params.rgrc[2].top    = 0;
  params.rgrc[2].right  = 0;
  params.rgrc[2].bottom = 0;

  auto result = wm.windowClass.wndProc(
      window.nativeHandle, WM_NCCALCSIZE, TRUE,
      // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
      reinterpret_cast<LPARAM>(&params));
  if (result == 0) {
    window.rect.x      = params.rgrc[0].left;
    window.rect.y      = params.rgrc[0].top;
    window.rect.width  = params.rgrc[0].right - params.rgrc[0].left;
    window.rect.height = params.rgrc[0].bottom - params.rgrc[0].top;
  } else {
    window.rect = window.frameRect - WindowManager::getEdges(window.style);
  }
}

BOOL GetWindowRect_mocked(HWND hWnd, LPRECT lpRect) {
  if (hWnd == nullptr || lpRect == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  lpRect->top    = window.frameRect.y;
  lpRect->right  = window.frameRect.x + window.frameRect.width;
  lpRect->bottom = window.frameRect.y + window.frameRect.height;
  lpRect->left   = window.frameRect.x;
  return TRUE;
}

BOOL GetClientRect_mocked(HWND hWnd, LPRECT lpRect) {
  if (hWnd == nullptr || lpRect == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  lpRect->top    = 0;
  lpRect->right  = window.rect.width;
  lpRect->bottom = window.rect.height;
  lpRect->left   = 0;
  return TRUE;
}

BOOL SetWindowTextW_mocked(HWND hWnd, LPCWSTR lpString) {
  if (hWnd == nullptr || lpString == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  window.title = ::yaul::WideChar::rev(lpString);
  return TRUE;
}

BOOL ShowWindow_mocked(HWND hWnd, int nCmdShow) {
  if (hWnd == nullptr)
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  BOOL previouslyVisible =
      (window.showState == ::yaul::Window::ShowState::maximize ||
       window.showState == ::yaul::Window::ShowState::restore)
          ? TRUE
          : FALSE;

  WPARAM beingShown = FALSE;
  switch (nCmdShow) {
    case SW_FORCEMINIMIZE:
    case SW_MINIMIZE:
    case SW_SHOWMINIMIZED:
    case SW_SHOWMINNOACTIVE:
      if (window.showState == ::yaul::Window::ShowState::restore)
        window.restoreRect = window.frameRect;
      window.showState = ::yaul::Window::ShowState::minimize;
      window.frameRect = ::yaul::Rectangle{};
      window.rect      = ::yaul::Rectangle{};
      beingShown       = FALSE;
      break;
    case SW_MAXIMIZE:
      // case SW_SHOWMAXIMIZED: same ordinal as SW_MAXIMIZE
      if (window.showState == ::yaul::Window::ShowState::restore)
        window.restoreRect = window.frameRect;
      window.showState = ::yaul::Window::ShowState::maximize;
      window.frameRect = MonitorMock::monitors.front().rect;
      calculateClientArea(hWnd);
      beingShown = TRUE;
      break;
    case SW_RESTORE:
    case SW_SHOWNORMAL:
    case SW_SHOWNOACTIVATE:
      window.showState = ::yaul::Window::ShowState::restore;
      SetWindowPos(hWnd, nullptr, window.restoreRect.x, window.restoreRect.y,
                   window.restoreRect.width, window.restoreRect.height,
                   SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS);
      beingShown = TRUE;
      break;
    case SW_HIDE:
      if (window.showState == ::yaul::Window::ShowState::restore)
        window.restoreRect = window.frameRect;
      window.showState = ::yaul::Window::ShowState::hidden;
      window.frameRect = ::yaul::Rectangle{};
      window.rect      = ::yaul::Rectangle{};
      beingShown       = FALSE;
      break;
    case SW_SHOW:
    case SW_SHOWDEFAULT:
    case SW_SHOWNA:
      // Shows window in the current size and position
      beingShown = static_cast<WPARAM>(previouslyVisible);
      break;
  }

  wm.windowClass.wndProc(hWnd, WM_SHOWWINDOW, beingShown, 0);

  return previouslyVisible;
}

BOOL ShowWindowAsync_mocked(HWND hWnd, int nCmdShow) {
  WindowManager::instance().asyncQueue.emplace_back(
      [hWnd, nCmdShow] { ShowWindow(hWnd, nCmdShow); });
  return TRUE;
}

HRESULT DwmIsCompositionEnabled_mocked(BOOL* pfEnabled) {
  if (pfEnabled == nullptr)
    return S_FALSE;

  *pfEnabled = static_cast<BOOL>(WindowManager::instance().compositionEnabled);
  return S_OK;
}

HRESULT DwmExtendFrameIntoClientArea_mocked(HWND hWnd,
                                            const MARGINS* pMarInset) {
  if (hWnd == nullptr || pMarInset == nullptr)
    return S_FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return S_FALSE;
  auto& window = itr->second;

  window.frameExtendedIntoClientArea =
      (pMarInset->cxLeftWidth > 0 || pMarInset->cxRightWidth > 0 ||
       pMarInset->cyBottomHeight > 0 || pMarInset->cyTopHeight > 0);
  return S_OK;
}

LRESULT DefWindowProcW_mocked(HWND hWnd,
                              UINT Msg,
                              WPARAM wParam,
                              LPARAM lParam) {
  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end()) {
    std::cerr << "Unrecognized hWnd to DefWindowProcW_mocked" << std::endl;
    WindowManager::instance().anyErrors = true;
    return 0;
  }
  auto& window = itr->second;

  switch (Msg) {
    case WM_NCCALCSIZE: {
      // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
      auto& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
      if (wParam == TRUE) {
        auto edges = WindowManager::getEdges(window.style);
        params.rgrc[0].top += edges.top;
        params.rgrc[0].right -= edges.right;
        params.rgrc[0].bottom -= edges.bottom;
        params.rgrc[0].left += edges.left;
        return 0;
      }
    } break;
    case WM_NCHITTEST: {
      ::yaul::Position cursor{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
      if (!(cursor && window.frameRect))
        return HTNOWHERE;

      auto frameEdges = static_cast<::yaul::Edges>(window.frameRect);
      bool inCaption  = false;
      if ((window.style & WS_CAPTION) == WS_CAPTION &&
          cursor.y > (frameEdges.top + SystemMetricsMocked::cYBorder) &&
          cursor.y < (frameEdges.top + SystemMetricsMocked::cYCaption +
                      SystemMetricsMocked::cYResizeHandle)) {
        constexpr int buttonWidth = 45;
        if (cursor.x <
            (frameEdges.left - SystemMetricsMocked::cXResizeHandle)) {
          if (cursor.x >
              (frameEdges.left - SystemMetricsMocked::cXResizeHandle -
               buttonWidth * 1))
            return HTCLOSE;
          if (cursor.x >
              (frameEdges.left - SystemMetricsMocked::cXResizeHandle -
               buttonWidth * 2))
            return HTMAXBUTTON;
          if (cursor.x >
              (frameEdges.left - SystemMetricsMocked::cXResizeHandle -
               buttonWidth * 3))
            return HTMINBUTTON;
        }
        inCaption = true;
      }

      if ((window.style & WS_SIZEBOX) == WS_SIZEBOX) {
        enum EdgeMask : uint8_t {
          top    = 0x01,
          right  = 0x02,
          bottom = 0x04,
          left   = 0x08
        };

        // clang-format off
        auto edgeMask =
            (cursor.y < (frameEdges.top     + SystemMetricsMocked::cYResizeHandle) ? top     : 0) |
            (cursor.x > (frameEdges.right   - SystemMetricsMocked::cXResizeHandle) ? right   : 0) |
            (cursor.y > (frameEdges.bottom  - SystemMetricsMocked::cYResizeHandle) ? bottom  : 0) |
            (cursor.x < (frameEdges.left    + SystemMetricsMocked::cXResizeHandle) ? left    : 0);
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

      if (inCaption)
        return HTCAPTION;

      if (cursor && window.rect)
        return HTCLIENT;

      return HTBORDER;
    } break;
    case WM_DESTROY:
    case WM_NCDESTROY:
    case WM_SHOWWINDOW:
      return 0;
  }

  std::cerr << "Unrecognized call to DefWindowProcW_mocked: " << Msg
            << std::endl;
  WindowManager::instance().anyErrors = true;
  return 0;
}

WindowManager::WindowManager() noexcept {
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  handles.hModule = reinterpret_cast<HMODULE>(rand());
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  handles.arrow = reinterpret_cast<HCURSOR>(rand());
}

::yaul::Edges WindowManager::getEdges(DWORD dwStyle) noexcept {
  ::yaul::Edges edges;
  switch (dwStyle & (WS_BORDER | WS_SIZEBOX | WS_DLGFRAME)) {
    case WS_BORDER:
      edges = ::yaul::Edges{
          SystemMetricsMocked::cYBorder, SystemMetricsMocked::cXBorder,
          SystemMetricsMocked::cYBorder, SystemMetricsMocked::cXBorder};
      break;
    case WS_SIZEBOX:
    case WS_BORDER | WS_SIZEBOX:
    case WS_DLGFRAME | WS_SIZEBOX:
      edges = ::yaul::Edges{SystemMetricsMocked::cYResizeHandle,
                            SystemMetricsMocked::cXResizeHandle,
                            SystemMetricsMocked::cYResizeHandle,
                            SystemMetricsMocked::cXResizeHandle};
      break;
    case WS_CAPTION:
    case WS_CAPTION | WS_SIZEBOX:
      edges = ::yaul::Edges{
          SystemMetricsMocked::cYResizeHandle + SystemMetricsMocked::cYCaption,
          SystemMetricsMocked::cXResizeHandle,
          SystemMetricsMocked::cYResizeHandle,
          SystemMetricsMocked::cXResizeHandle};
      break;
    case WS_DLGFRAME:
      edges = ::yaul::Edges{
          SystemMetricsMocked::cYFixedFrame, SystemMetricsMocked::cXFixedFrame,
          SystemMetricsMocked::cYFixedFrame, SystemMetricsMocked::cXFixedFrame};
      break;
    default:
      break;
  }
  return edges;
}

LRESULT WindowManager::sendHitTest(HWND hWnd, ::yaul::Position cursor) {
  if (hWnd == nullptr)
    return E_HANDLE;

  auto itr = windows.find(hWnd);
  if (itr == windows.end())
    return E_HANDLE;
  auto& window = itr->second;

  if (!(cursor && window.frameRect))
    return HTNOWHERE;

  return windowClass.wndProc(hWnd, WM_NCHITTEST, 0,
                             MAKELONG(cursor.x, cursor.y));
}

void WindowManager::sendEventToAll(Event event) {
  UINT msg      = WM_NULL;
  WPARAM wParam = 0;
  LPARAM lParam = 0;
  switch (event) {
    case Event::close:
      msg = WM_CLOSE;
      break;
    case Event::maximize: {
      msg          = WM_SIZE;
      wParam       = SIZE_MAXIMIZED;
      auto monitor = MonitorMock::monitors.front().rect;
      lParam       = MAKELONG(monitor.width, monitor.height);
      for (auto& itr : windows) {
        itr.second.showState = ::yaul::Window::ShowState::maximize;
      }
    } break;
    case Event::minimize:
      msg    = WM_SIZE;
      wParam = SIZE_MINIMIZED;
      lParam = 0;
      for (auto& itr : windows) {
        itr.second.showState = ::yaul::Window::ShowState::minimize;
      }
      break;
    case Event::restore: {
      msg          = WM_SIZE;
      wParam       = SIZE_RESTORED;
      auto monitor = MonitorMock::monitors.front().rect;
      lParam       = MAKELONG(monitor.width / 2, monitor.height / 2);
      for (auto& itr : windows) {
        itr.second.showState = ::yaul::Window::ShowState::restore;
      }
    } break;
  }
  if (msg == WM_NULL)
    return;
  for (auto& itr : windows) {
    windowClass.wndProc(itr.first, msg, wParam, lParam);
  }
}

BOOL GetWindowPlacement_mocked(HWND hWnd, WINDOWPLACEMENT* lpwndpl) {
  if (hWnd == nullptr || lpwndpl == nullptr)
    return FALSE;

  if (lpwndpl->length != sizeof(WINDOWPLACEMENT))
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;
  auto& window = itr->second;

  lpwndpl->flags = 0;

  ::yaul::Rectangle normalPosition = window.frameRect;

  switch (window.showState) {
    case ::yaul::Window::ShowState::hidden:
    case ::yaul::Window::ShowState::restore:
      lpwndpl->showCmd = SW_SHOWNORMAL;
      break;
    case ::yaul::Window::ShowState::maximize:
      lpwndpl->showCmd = SW_SHOWMAXIMIZED;
      normalPosition   = window.restoreRect;
      break;
    case ::yaul::Window::ShowState::minimize:
      lpwndpl->showCmd = SW_SHOWMINIMIZED;
      normalPosition   = window.restoreRect;
      break;
  }

  // Top left corner when minimized, ignore
  lpwndpl->ptMinPosition.x = -1;
  lpwndpl->ptMinPosition.y = -1;

  // Top left corner when maximized, ignore
  lpwndpl->ptMaxPosition.x = -1;
  lpwndpl->ptMaxPosition.y = -1;

  // Coordinates when restored
  lpwndpl->rcNormalPosition.left   = normalPosition.x;
  lpwndpl->rcNormalPosition.top    = normalPosition.y;
  lpwndpl->rcNormalPosition.right  = normalPosition.x + normalPosition.width;
  lpwndpl->rcNormalPosition.bottom = normalPosition.y + normalPosition.height;

  return TRUE;
}

BOOL SetWindowPlacement_mocked(HWND hWnd, const WINDOWPLACEMENT* lpwndpl) {
  if (hWnd == nullptr || lpwndpl == nullptr)
    return FALSE;

  if (lpwndpl->length != sizeof(WINDOWPLACEMENT))
    return FALSE;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hWnd);
  if (itr == wm.windows.end())
    return FALSE;

  if (SetWindowPos(
          hWnd, nullptr, lpwndpl->rcNormalPosition.left,
          lpwndpl->rcNormalPosition.top,
          lpwndpl->rcNormalPosition.right - lpwndpl->rcNormalPosition.left,
          lpwndpl->rcNormalPosition.bottom - lpwndpl->rcNormalPosition.top,
          SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_ASYNCWINDOWPOS) == FALSE)
    return FALSE;
  if (ShowWindowAsync(hWnd, static_cast<int>(lpwndpl->showCmd)) == FALSE)
    return FALSE;
  return TRUE;
}

HMONITOR MonitorFromWindow_mocked(HWND hwnd, DWORD dwFlags) {
  if (hwnd == nullptr)
    return nullptr;

  auto& wm = WindowManager::instance();

  auto itr = wm.windows.find(hwnd);
  if (itr == wm.windows.end())
    return nullptr;
  auto& window = itr->second;

  HMONITOR hMonitor = nullptr;

  int maxArea = 0;
  for (const auto& monitor : MonitorMock::monitors) {
    auto area = (monitor.rect & window.frameRect).area();
    if (area > maxArea) {
      maxArea = area;
      // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
      hMonitor = reinterpret_cast<HMONITOR>(
          // NOLINTNEXTLINE (cppcoreguidelines-pro-type-const-cast)
          const_cast<MonitorMock::MonitorInfo*>(&monitor));
    }
  }
  if (hMonitor != nullptr)
    return hMonitor;

  if (dwFlags == MONITOR_DEFAULTTONULL) {
    hMonitor = nullptr;
  } else if (dwFlags == MONITOR_DEFAULTTOPRIMARY) {
    for (const auto& monitor : MonitorMock::monitors) {
      if (monitor.primary) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
        hMonitor = reinterpret_cast<HMONITOR>(
            // NOLINTNEXTLINE (cppcoreguidelines-pro-type-const-cast)
            const_cast<MonitorMock::MonitorInfo*>(&monitor));
      }
    }
  } else if (dwFlags == MONITOR_DEFAULTTONEAREST) {
    // TODO (WattsUp) measure distances and get nearest
  }
  return hMonitor;
}

BOOL GetMonitorInfoW_mocked(HMONITOR hMonitor, LPMONITORINFO lpmi) {
  if (hMonitor == nullptr || lpmi == nullptr)
    return FALSE;

  if (lpmi->cbSize != sizeof(MONITORINFO))
    return FALSE;

  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
  auto* monitor = reinterpret_cast<MonitorMock::MonitorInfo*>(hMonitor);

  lpmi->rcMonitor.left   = monitor->rect.x;
  lpmi->rcMonitor.top    = monitor->rect.y;
  lpmi->rcMonitor.right  = monitor->rect.x + monitor->rect.width;
  lpmi->rcMonitor.bottom = monitor->rect.y + monitor->rect.height;

  // No taskbar
  lpmi->rcWork = lpmi->rcMonitor;

  if (monitor->primary) {
    lpmi->dwFlags = MONITORINFOF_PRIMARY;
  } else {
    lpmi->dwFlags = 0;
  }

  return TRUE;
}