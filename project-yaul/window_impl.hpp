#ifndef YAUL_WINDOW_IMPL_HPP
#define YAUL_WINDOW_IMPL_HPP

#include <yaul/window.hpp>

#include "common/string.hpp"
#include "object_impl.hpp"

#include <atomic>
#include <mutex>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#include <windowsx.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

constexpr UINT YAUL_WM_NEW_WINDOW = (WM_USER + 0);
constexpr UINT YAUL_WM_STOP_LOOP  = (WM_USER + 1);
#endif /* WIN32 */

namespace yaul {

class Window::Impl final : public SharedObject::Impl {
 public:
  /**
   * @brief Construct a new Window implementation object with default values
   *
   * @param size in pixels (inner size, see setSize)
   * @param title to display on titlebar
   * @param state true will show the window, false will not, use setShowState to
   * @throws std::exception if failed to create native window
   */
  Impl(Size size,
       const char* title,
       ShowState state = ShowState::restore) noexcept(false);

  YAUL_DEFINE_DESTRUCT(Impl);
  YAUL_NO_COPY(Impl);
  YAUL_NO_MOVE(Impl);

  /**
   * @brief Check if a close signal is present for the window
   *
   * @return true when the window has been signaled to close, false otherwise
   */
  bool shouldClose() const noexcept;

  /**
   * @brief Render any pending commands to the backbuffer then swap to present
   * it to the screen
   *
   */
  void render() noexcept;

  /**
   * @brief Check and process pending events for the window, namely input
   * keypresses or mouse actions
   *
   */
  void pollEvents() noexcept;

  /**
   * @brief Set the size of the window, both dimensions must be non-zero
   * Dimensions include the size of the border if present (see setBorderless)
   * and innerSize is false
   *
   * @param size in pixels
   * @param innerSize true indicates width and height exclude border dimensions.
   * only applicable if window has a border, see setBorderless
   * @return true if size was successfully adjusted, false otherwise
   */
  bool setSize(Size size, bool innerSize = true) noexcept;

  /**
   * @brief Get the size of the window
   *
   * @param innerSize true will exclude border if present, false will include
   * border if present
   * @return Size in pixels
   */
  Size getSize(bool innerSize = true) const noexcept;

  /**
   * @brief Set the position of the window
   *
   * @param position distance between top edges and left edges of monitor and
   * window, -1 will center respective axis
   * @param monitor to position relative to
   * @return true if position was successfully adjusted, false otherwise
   */
  bool setPosition(Position position,
                   const Monitor* monitor = nullptr) noexcept;

  /**
   * @brief Set the window to be fullscreen or not
   *
   * @param fullscreen true will make the window fill the monitor, false will
   * restore it to its previous state
   * @param monitor to full screen into, default is the current one the window
   * is on
   */
  void setFullscreen(bool fullscreen,
                     const Monitor* monitor = nullptr,
                     bool lockMutex         = true) noexcept;

  /**
   * @brief Set the title of the window.
   *
   * @param title
   * @param lockMutex true will lock the mutex during operation
   */
  void setTitle(const char* title, bool lockMutex = true) noexcept;

  /**
   * @brief Set the window resizeable or not
   *
   * @param resizable true will allow resizing via border cursor dragging
   * @param lockMutex true will lock the mutex during operation
   */
  void setResizable(bool resizable, bool lockMutex = true) noexcept;

  /**
   * @brief Set the width of the resizing border, a transparent area around the
   * window that captures mouse event for resizing.
   *
   * @param border width of each edge's border in pixels
   * @param lockMutex true will lock the mutex during operation
   */
  void setResizingBorder(Edges border, bool lockMutex = true) noexcept;

  /**
   * @brief Set the window to be borderless or not. Border includes title and
   * sizing buttons
   *
   * @param borderless true will remove border (and titlebar/buttons), false
   * will restore border
   * @param lockMutex true will lock the mutex during operation
   */
  void setBorderless(bool borderless, bool lockMutex = true) noexcept;

  /**
   * @brief Set the window to have a shadow or not when borderless
   *
   * @param shadow true will draw a shadow around the window when borderless,
   * false will not
   * @param lockMutex true will lock the mutex during operation
   */
  void setBorderlessShadow(bool shadow, bool lockMutex = true) noexcept;

  /**
   * @brief Set the dragging area of the window for changing its position. A
   * bordered window default to the title bar. Area defined by rectangle with
   * corners (0, 0) and (window width - menuWidth, bottom). To further mask this
   * area (such as buttons), see addDraggingAreaMask()
   *
   * @param bottom in pixels distance from top of window to bottom of area.
   * @param menuWidth in pixels width of menu (minimize, maximize, close)
   * @param lockMutex true will lock the mutex during operation
   */
  void setDraggingArea(int bottom,
                       int menuWidth,
                       bool lockMutex = true) noexcept;

  /**
   * @brief Clear the list of dragging area masks added by addDraggingAreaMask
   *
   * @param lockMutex true will lock the mutex during operation
   */
  void clearDraggingAreaMasks(bool lockMutex = true) noexcept;

  /**
   * @brief Add a mask to the dragging area defined by setDraggingArea. To clear
   * the list of masks, see clearDraggingAreaMasks
   *
   * @param rect to mask, coordinates are relative to top left (0, 0)
   * @param lockMutex true will lock the mutex during operation
   */
  void addDraggingAreaMask(Rectangle rect, bool lockMutex = true) noexcept;

  /**
   * @brief Set the show state of the window, see ShowState
   *
   * @param state
   * @param lockMutex true will lock the mutex during operation
   */
  void setShowState(ShowState state, bool lockMutex = true) noexcept;

  /**
   * @brief Request the window be closed
   *
   */
  inline void closeRequest() noexcept { closeFlag = true; }

 private:
  /**
   * @brief Create a the native window via OS specific functions
   *
   * @throws std::exception if failed to create native window
   */
  void createNativeWindow() noexcept(false);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  static LRESULT CALLBACK windowProcedure(HWND hWindow,
                                          UINT msg,
                                          WPARAM wParam,
                                          LPARAM lParam);

  /**
   * @brief Check if composition is enabled required for aero theming
   *
   * @return true when composition is enabled, false otherwise
   */
  static bool compositionEnabled() noexcept;

  /**
   * @brief Adjust the client area rectangle. When maximized, fill the working
   * area of the current monitor.
   *
   * @param window to adjust
   * @param rect to adjust
   */
  static void adjustClientRect(HWND window, RECT* rect) noexcept;

  /**
   * @brief Perform a hit test between the cursor and the window looking for
   * resize or drag events
   *
   * @param cursor location
   * @return LRESULT
   * - HTNOWHERE no collision
   * - HTCAPTION draggable area
   * - HTCLIENT  client area
   * - HTTOP, HTTOPRIGHT, HTRIGHT, HTBOTTOMRIGHT, HTBOTTOM, HTBOTTOMLEFT,
   * HTLEFT, HTTOPLEFT resizing area
   * @param lockMutex true will lock the mutex during operation
   */
  LRESULT hitTest(Position cursor, bool lockMutex = true) noexcept;

  /**
   * @brief Display the system menu and send selected command
   *
   * @param position to place menu at
   */
  void showSysMenu(Position position) const noexcept;

  /**
   * @brief Enable/disable a menu item by command ID
   *
   * @param menu to modify
   * @param command to modify
   * @param enabled true will enable, false will disable and grey out
   */
  static inline void enableMenuItem(HMENU menu,
                                    UINT command,
                                    bool enabled) noexcept {
    ::EnableMenuItem(
        menu, command,
        MF_BYCOMMAND | (enabled ? MF_ENABLED : MF_DISABLED | MF_GRAYED));
  }

  using NativeWindow = HWND;

  static HINSTANCE registerClass() noexcept;
  static const HINSTANCE hInstance;

  /**
   * @enum Windows window styles
   *
   * - WS_THICKFRAME for sizing border
   * - WS_SYSMENU for indow menu (right click title bar or shift-right click
   * taskbar)
   * - WS_MINIMIZEBOX for minimize enabled
   * - WS_MAXIMIZEBOX for maximize enabled
   * - WS_CAPTION for aero transitions and title bar (drawn over for borderless)
   * - WS_OVERLAPPED for normal window
   * - WS_POPUP for borderless windows
   */
  enum class WindowStyle : DWORD {
    base = WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,

    windowed        = base | WS_CAPTION | WS_OVERLAPPED,
    aeroBorderless  = base | WS_CAPTION | WS_POPUP,
    basicBorderless = base | WS_POPUP
  };

#endif /* WIN32 */

  NativeWindow nativeWindow = nullptr;

  std::atomic<bool> closeFlag = false;

  bool fullscreen = false;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  LONG_PTR fullscreenPreStyle = 0;
  WINDOWPLACEMENT fullscreenPrePlacement{};
#endif /* WIN32 */

  // Lock data used by setting thread, windproc, and renderer before setting or
  // reading. That includes all member variables below
  std::mutex mutex;

  string title;

  ShowState showState;
  bool resizable        = true;
  bool borderless       = false;
  bool borderlessShadow = true;

  Edges resizingBorder {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER)
#endif /* WIN32 */
  };

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  int draggingAreaBottom = ::GetSystemMetrics(SM_CYSIZE);
  int menuWidth          = ::GetSystemMetrics(SM_CXSIZE) * 3;
#endif /* WIN32 */
  std::list<Rectangle> draggingAreaMasks;
};

}  // namespace yaul

#endif /* _YAUL_WINDOW_IMPL_HPP_ */