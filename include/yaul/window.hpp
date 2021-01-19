#ifndef _YAUL_WINDOW_HPP_
#define _YAUL_WINDOW_HPP_

#include <yaul/dimensions.hpp>
#include <yaul/object.hpp>

namespace yaul {

// Forward declaration for friend status
// namespace Application::Impl{
class Application;
// }

class YAUL_API Window final : public SharedObject {
 public:
  /**
   * @enum Window show state
   *
   */
  enum class ShowState : uint8_t {
    hidden = 0,    ///< Hide the window, not available on the taskbar. Other
                   ///< states revert this
    minimize = 1,  ///< Minimize the window to the taskbar
    maximize = 2,  ///< Maximize the window to fill the current monitor
    restore  = 3,  ///< Restore the window to its previous size and position
  };

  /**
   * @brief Construct a new Window with given parameters
   *
   * @param size in pixels (inner size, see setSize)
   * @param title to display on titlebar
   * @param state true will show the window, false will not, use setShowState to
   *
   * @throws std::exception if failed to create window
   */
  Window(Size size, const char* title, ShowState state) noexcept(false) {
    YAUL_EXCEPTION_WRAPPER_THROW_VOID(apiCreateWindow, size, title, state);
  }

  YAUL_DEFINE_DESTRUCT_MOVE_COPY(Window);

  class Impl;

  /**
   * @brief Check if a close signal is present for the window
   *
   * @return true when the window has been signaled to close, false otherwise
   */
  bool shouldClose() noexcept;

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
   * @brief Request the window be closed
   *
   */
  void closeRequest() noexcept;

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
  [[nodiscard]] Size getSize(bool innerSize = true) const noexcept;

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
   * @brief Set the window to be fullscreen or not
   *
   * @param fullscreen true will make the window fill the monitor, false will
   * restore it to its previous state
   * @param monitor to full screen into, default is the current one the window
   * is on
   */
  void setFullscreen(bool fullscreen, int monitor = 0) noexcept;

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
   * @brief Set the window to have a shadow or not when borderless
   *
   * @param shadow true will draw a shadow around the window when borderless,
   * false will not
   */
  void setBorderlessShadow(bool shadow) noexcept;

  /**
   * @brief Set the dragging area of the window for changing its position. A
   * bordered window default to the title bar. Area defined by rectangle filling
   * the top area with a bottom edge defined by distance from top. Dragging is
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

  friend class Application;  // For Window()

 private:
  /**
   * @brief Construct an empty window object. SharedObject points to nullptr
   *
   */
  Window() noexcept;

  /**
   * @brief Construct a new Window with given parameters. If successful, the
   * pImpl points to the new window
   *
   * @param size in pixels (inner size, see setSize)
   * @param title to display on titlebar
   * @param state true will show the window, false will not, use setShowState to
   * @param r result object returned
   */
  void apiCreateWindow(Size size,
                       const char* title,
                       ShowState state,
                       Result* r) noexcept;
};

}  // namespace yaul

#endif /* _YAUL_WINDOW_HPP_ */