#ifndef _YAUL_WINDOW_IMPL_HPP_
#define _YAUL_WINDOW_IMPL_HPP_

#include <yaul/window.hpp>

#include "common/string.hpp"
#include "object_impl.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>
#endif /* WIN32 */

namespace yaul {

class Window::Impl final : public Object::Impl {
 public:
  /**
   * @brief Construct a new Window implementation object with default values
   *
   * @param size in pixels (total size including border)
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
   * @brief Create a the native window via OS specific functions
   *
   * @throws std::exception if failed to create native window
   */
  void createNativeWindow(Size size) noexcept(false);

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
  bool setSize(Size size, bool innerSize = false) noexcept;

  /**
   * @brief Get the size of the window
   *
   * @param innerSize true will exclude border if present, false will include
   * border if present
   * @return Size in pixels
   */
  Size getSize(bool innerSize = false) const noexcept;

  /**
   * @brief Set the position of the window
   *
   * @param position distance between top edges and left edges of monitor and
   * window, -1 will center respective axis
   * @param monitor to position relative to
   * @return true if position was successfully adjusted, false otherwise
   */
  bool setPosition(Position position, int monitor = 0) noexcept;

  /**
   * @brief Set the title of the window.
   *
   * @param title
   */
  void setTitle(const char* title) noexcept;

  /**
   * @brief Set the window resizeable or not
   *
   * @param resizable true will allow resizing via border cursor dragging
   */
  void setResizable(bool resizable) noexcept;

  /**
   * @brief Set the width of the resizing border, a transparent area around the
   * window that captures mouse event for resizing.
   *
   * @param border width of each edge's border in pixels
   */
  void setResizingBorder(Edges border) noexcept;

  /**
   * @brief Set the window to be borderless or not. Border includes title and
   * sizing buttons
   *
   * @param borderless true will remove border (and titlebar/buttons), false
   * will restore border
   */
  void setBorderless(bool borderless) noexcept;

  /**
   * @brief Set the window draggable or not to change its position
   *
   * @param draggable true will allow dragging via dragging area, see
   * setDraggingArea
   */
  void setDraggable(bool draggable) noexcept;

  /**
   * @brief Set the dragging area of the window for changing its position. A
   * bordered window default to the title bar. Area defined by rectangle filling
   * the top with a bottom edge defined by distance from top. Dragging is
   * lower than other interactions (buttons, resizing) such that those regions
   * mask this draggable area.
   *
   * @param bottom in pixels distance from top of window to bottom of area.
   */
  void setDraggingArea(int bottom) noexcept;

  /**
   * @brief Set the show state of the window, see ShowState
   *
   * @param state
   */
  void setShowState(ShowState state) noexcept;

  /**
   * @brief Request the window be closed
   *
   */
  inline void closeRequest() noexcept { closeFlag = true; }

 private:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  typedef HWND NativeWindow;
  static HINSTANCE hInstance;
#endif /* WIN32 */

  NativeWindow nativeWindow = nullptr;

  string title;

  bool closeFlag  = false;
  bool resizable  = true;
  bool borderless = false;
  bool draggable  = false;

  Edges resizingBorder {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER),
        ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER)
#endif /* WIN32 */
  };

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  int draggingAreaBottom = ::GetSystemMetrics(SM_CYMENU);
#endif /* WIN32 */
};

}  // namespace yaul

#endif /* _YAUL_WINDOW_IMPL_HPP_ */