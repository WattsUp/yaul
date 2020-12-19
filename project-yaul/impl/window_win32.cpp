#include "window.hpp"

namespace yaul {

const LPCWSTR windowClassName = L"yaul";

LRESULT CALLBACK windowProcedure(HWND hwnd,
                                 UINT msg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
  // Process messages
  switch (msg) {
    case WM_CREATE:
      break;
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
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
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  return 0;
}

namespace impl {

void Window::createWindow(const string& id) {
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  WNDCLASSW wc;
  ZeroMemory(&wc, sizeof(WNDCLASSW));

  wc.lpszClassName = windowClassName;
  wc.lpfnWndProc   = windowProcedure;
  wc.hInstance     = hInstance;
  if (RegisterClassW(&wc) == 0) {
    throw std::exception("Failed to register class");
  }

  window = CreateWindowW(windowClassName, windowClassName, WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                         CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);
  if (window == nullptr) {
    throw std::exception("Failed to create window");
  }

  SetWindowTextA(window, id.c_str());
}

}  // namespace impl
}  // namespace yaul