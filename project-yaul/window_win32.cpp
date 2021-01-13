#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

HINSTANCE Window::Impl::hInstance = nullptr;

const LPCWSTR windowClassName = L"yaul";

LRESULT CALLBACK windowProcedure(HWND hwnd,
                                 UINT msg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
  auto* window = static_cast<Window::Impl*>(::GetPropW(hwnd, L"yaul"));
  if (window == nullptr) {
    return ::DefWindowProcW(hwnd, msg, wParam, lParam);
  }

  switch (msg) {
    case WM_CREATE:
      break;
    case WM_CLOSE:
      window->closeRequest();
      break;
    case WM_MOVE:
      break;
    case WM_PAINT: {
      // GDI - Graphics Devices Interface Here
      //--------------------------------------------
      PAINTSTRUCT ps;
      HDC hdc = nullptr;
      // std::cerr << " [INFO] Windown painting" << std::endl;
      hdc         = BeginPaint(hwnd, &ps);
      string text = "Hello world Window!";
      TextOutA(hdc, 0, 0, text.c_str(), static_cast<int>(text.size()));
      EndPaint(hwnd, &ps);
    } break;
    default:
      return ::DefWindowProcW(hwnd, msg, wParam, lParam);
  }

  return 0;
}

void Window::Impl::createNativeWindow(Size size) noexcept(false) {
  if (hInstance == nullptr) {
    hInstance = ::GetModuleHandleW(nullptr);

    WNDCLASSW wc;
    ::ZeroMemory(&wc, sizeof(WNDCLASSW));

    wc.lpszClassName = windowClassName;
    wc.lpfnWndProc   = windowProcedure;
    wc.hInstance     = hInstance;
    if (::RegisterClassW(&wc) == 0) {
      throw std::exception("Failed to register class");
    }
  }

  nativeWindow = ::CreateWindowExW(
      WS_EX_ACCEPTFILES, windowClassName, windowClassName, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, size.width, size.height, nullptr, nullptr,
      hInstance, nullptr);
  if (nativeWindow == nullptr) {
    throw std::exception("Failed to create window");
  }

  ::SetPropW(nativeWindow, L"yaul", this);
}

}  // namespace yaul