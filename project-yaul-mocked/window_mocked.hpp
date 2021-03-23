#ifndef YAUL_WINDOW_MOCKED_HPP
#define YAUL_WINDOW_MOCKED_HPP

#include <yaul/common.hpp>
#include <yaul/dimensions.hpp>

#include <yaul/window.hpp>

#include "common/string.hpp"

#include "monitor_mocked.hpp"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

namespace SystemMetricsMocked {
constexpr int cXBorder = 1;
constexpr int cYBorder = 1;

constexpr int cXFixedFrame = 3;
constexpr int cYFixedFrame = 3;

constexpr int cXFrame = 4;
constexpr int cYFrame = 4;

constexpr int cXSizeFrame = 4;
constexpr int cYSizeFrame = 4;

constexpr int cXPaddedBorder = 4;

constexpr int cYCaption  = 23;
constexpr int cYMenu     = 20;
constexpr int cYMenuSize = 19;

constexpr int cXSize = 36;
constexpr int cYSize = 22;

constexpr int cXMin = 136;
constexpr int cYMin = 39;

constexpr int cXResizeHandle = cXFrame + cXSizeFrame;
constexpr int cYResizeHandle = cYFrame + cYSizeFrame;
};  // namespace SystemMetricsMocked

#define WIN32_LEAN_AND_MEAN
#include <dwmapi.h>
#include <windows.h>
#include <windowsx.h>

#undef GetSystemMetrics
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetSystemMetrics GetSystemMetrics_mocked
int GetSystemMetrics_mocked(int nIndex);

#undef EnableMenuItem
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define EnableMenuItem EnableMenuItem_mocked
BOOL EnableMenuItem_mocked(HMENU hMenu, UINT uIDEnableItem, UINT uEnable);

#undef GetModuleHandleW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetModuleHandleW GetModuleHandleW_mocked
HMODULE GetModuleHandleW_mocked(LPCWSTR lpModuleName);

#undef LoadCursorW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define LoadCursorW LoadCursorW_mocked
HCURSOR LoadCursorW_mocked(HINSTANCE hInstance, LPCWSTR lpCursorName);

#undef RegisterClassExW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define RegisterClassExW RegisterClassExW_mocked
ATOM RegisterClassExW_mocked(const WNDCLASSEXW* wc);

#undef CreateWindowExW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define CreateWindowExW CreateWindowExW_mocked
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
                            LPVOID lpParam);

#undef DestroyWindow
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define DestroyWindow DestroyWindow_mocked
BOOL DestroyWindow_mocked(HWND hWnd);

#undef SetPropW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define SetPropW SetPropW_mocked
BOOL SetPropW_mocked(HWND hWnd, LPCWSTR lpString, HANDLE hData);

#undef GetPropW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetPropW GetPropW_mocked
HANDLE GetPropW_mocked(HWND hWnd, LPCWSTR lpString);

#undef RemovePropW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define RemovePropW RemovePropW_mocked
HANDLE RemovePropW_mocked(HWND hWnd, LPCWSTR lpString);

#undef SetWindowLongPtrW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define SetWindowLongPtrW SetWindowLongPtrW_mocked
LONG_PTR SetWindowLongPtrW_mocked(HWND hWnd, int nIndex, LONG_PTR dwNewLong);

#undef GetWindowLongPtrW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetWindowLongPtrW GetWindowLongPtrW_mocked
LONG_PTR GetWindowLongPtrW_mocked(HWND hWnd, int nIndex);

#undef AdjustWindowRect
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define AdjustWindowRect AdjustWindowRect_mocked
BOOL AdjustWindowRect_mocked(LPRECT lpRect, DWORD dwStyle, BOOL bMenu);

#undef SetWindowPos
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define SetWindowPos SetWindowPos_mocked
BOOL SetWindowPos_mocked(HWND hWnd,
                         HWND hWndInsertAfter,
                         int X,
                         int Y,
                         int cx,
                         int cy,
                         UINT uFlags);

void calculateClientArea(HWND hWnd);

#undef GetWindowRect
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetWindowRect GetWindowRect_mocked
BOOL GetWindowRect_mocked(HWND hWnd, LPRECT lpRect);

#undef GetClientRect
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetClientRect GetClientRect_mocked
BOOL GetClientRect_mocked(HWND hWnd, LPRECT lpRect);

#undef SetWindowTextW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define SetWindowTextW SetWindowTextW_mocked
BOOL SetWindowTextW_mocked(HWND hWnd, LPCWSTR lpString);

#undef ShowWindow
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ShowWindow ShowWindow_mocked
BOOL ShowWindow_mocked(HWND hWnd, int nCmdShow);

#undef ShowWindowAsync
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define ShowWindowAsync ShowWindowAsync_mocked
BOOL ShowWindowAsync_mocked(HWND hWnd, int nCmdShow);

#undef DwmIsCompositionEnabled
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define DwmIsCompositionEnabled DwmIsCompositionEnabled_mocked
HRESULT DwmIsCompositionEnabled_mocked(BOOL* pfEnabled);

#undef DwmExtendFrameIntoClientArea
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define DwmExtendFrameIntoClientArea DwmExtendFrameIntoClientArea_mocked
HRESULT DwmExtendFrameIntoClientArea_mocked(HWND hWnd,
                                            const MARGINS* pMarInset);

#undef DefWindowProcW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define DefWindowProcW DefWindowProcW_mocked
LRESULT DefWindowProcW_mocked(HWND hWnd,
                              UINT Msg,
                              WPARAM wParam,
                              LPARAM lParam);

#undef GetWindowPlacement
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetWindowPlacement GetWindowPlacement_mocked
BOOL GetWindowPlacement_mocked(HWND hWnd, WINDOWPLACEMENT* lpwndpl);

#undef SetWindowPlacement
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define SetWindowPlacement SetWindowPlacement_mocked
BOOL SetWindowPlacement_mocked(HWND hWnd, const WINDOWPLACEMENT* lpwndpl);

#undef MonitorFromWindow
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define MonitorFromWindow MonitorFromWindow_mocked
HMONITOR MonitorFromWindow_mocked(HWND hwnd, DWORD dwFlags);

#undef GetMonitorInfoW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetMonitorInfoW GetMonitorInfoW_mocked
BOOL GetMonitorInfoW_mocked(HMONITOR hMonitor, LPMONITORINFO lpmi);

#undef PeekMessageW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define PeekMessageW PeekMessageW_mocked
BOOL PeekMessageW_mocked(LPMSG lpMsg,
                         HWND hWnd,
                         UINT wMsgFilterMin,
                         UINT wMsgFilterMax,
                         UINT wRemoveMsg);

#undef GetMessageW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define GetMessageW GetMessageW_mocked
BOOL GetMessageW_mocked(LPMSG lpMsg,
                        HWND hWnd,
                        UINT wMsgFilterMin,
                        UINT wMsgFilterMax);

#undef TranslateMessage
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define TranslateMessage TranslateMessage_mocked
BOOL TranslateMessage_mocked(const MSG* lpMsg);

#undef DispatchMessageW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define DispatchMessageW DispatchMessageW_mocked
LRESULT DispatchMessageW_mocked(const MSG* lpMsg);

#undef PostThreadMessageW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define PostThreadMessageW PostThreadMessageW_mocked
BOOL PostThreadMessageW_mocked(DWORD idThread,
                               UINT Msg,
                               WPARAM wParam,
                               LPARAM lParam);

#undef PostQuitMessage
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define PostQuitMessage PostQuitMessage_mocked
void PostQuitMessage_mocked(int nExitCode);

#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

class YAUL_API WindowManager {
 private:
  WindowManager() noexcept;

 public:
  ~WindowManager() noexcept = default;
  YAUL_NO_MOVE(WindowManager);
  YAUL_NO_COPY(WindowManager);

  /**
   * @brief Get the singleton window manager instance
   *
   * @return WindowManager
   */
  static WindowManager& instance(bool reset = false) noexcept {
    static WindowManager instance;
    if (reset) {
      // instance.windowClasses.clear() // Don't clear window classes
      instance.windowClass.threadID = std::thread::id();
      instance.windowClass.messages.clear();
      instance.windows.clear();
      instance.anyErrors = false;
      instance.asyncQueue.clear();
      instance.compositionEnabled = true;
    }
    return instance;
  }

  static ::yaul::Edges getEdges(DWORD dwStyle) noexcept;

  bool anyErrors = false;

  std::list<std::function<void()>> asyncQueue;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  struct Handles {
    HMODULE hModule = nullptr;

    HCURSOR arrow = nullptr;
  };
  Handles handles{};  // Do not dereference

  struct WindowClass {
    ATOM atom = 0;

    ::yaul::string className;

    WNDPROC wndProc = nullptr;

    // wndProc (DispatchMessageW), CreateWindow, and DestroyWindow need to be
    // all on the same thread. Save it on the first call to any of these
    std::thread::id threadID;
    DWORD threadNativeID = 0;

    std::list<MSG> messages;
  };
  WindowClass windowClass;

  std::mutex mutex;
  std::condition_variable cv;

  void postMessage(HWND hWnd,
                   UINT msg,
                   WPARAM wParam,
                   LPARAM lParam,
                   bool lockMutex = true);

  bool compositionEnabled = true;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

  struct Window {
    ::yaul::Rectangle rect;
    ::yaul::Rectangle frameRect;
    ::yaul::Rectangle restoreRect;
    ::yaul::string title;
    ::yaul::Window::ShowState showState = ::yaul::Window::ShowState::hidden;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    HWND nativeHandle = nullptr;  // Do not dereference

    DWORD style   = 0;
    DWORD exStyle = 0;

    std::unordered_map<::yaul::string, HANDLE> properties;

    bool frameExtendedIntoClientArea = false;
#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */
  };

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  std::unordered_map<HWND, Window> windows{};

  LRESULT sendHitTest(HWND hWnd, ::yaul::Position cursor);
#elif defined(__linux) || defined(__linux__)
  std::list<Window> windows{};
#endif /* WIN32, __linux__ */

  enum class Event { close, maximize, minimize, restore };

  void sendEventToAll(Event event);

  void processAsyncCalls() noexcept;
  void processMessages() noexcept;

  void waitForMessageQueue() noexcept;
};

#endif /* YAUL_WINDOW_MOCKED_HPP */