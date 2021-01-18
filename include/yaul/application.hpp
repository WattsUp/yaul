#ifndef _YAUL_APPLICATION_HPP_
#define _YAUL_APPLICATION_HPP_

#include <yaul/object.hpp>
#include <yaul/window.hpp>

namespace yaul {

/**
 * @brief Application settings
 *
 * @param logger function to direct log statements to
 * @param customRenderLoop yaul will not call Window.render and it is up to the
 * user to
 */
struct ApplicationSettings {
  logger_t logger       = nullptr;
  bool customRenderLoop = false;
};

constexpr Size defaultWindowSize{640, 480};

class YAUL_API Application final : public Object {
 public:
  /**
   * @brief Construct a yaul application
   *
   * @param argc command line argument count
   * @param argv command line arguments
   * @param settings of the application
   */
  Application(int argc,
              char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
              const ApplicationSettings& settings) noexcept;

  YAUL_DEFINE_DESTRUCT(Application);
  YAUL_NO_COPY(Application);
  YAUL_DEFINE_MOVE(Application);

  class Impl;

  /**
   * @brief Add a window to the application. If id already exists, this function
   * returns that window and does not create a new one
   *
   * @param id of the window to refer to in XML/CSS
   * @param size of the window, any XML/CSS styling will override this
   * immediately
   * @param showState of the window upon creation (any XML/CSS styling will be
   * applied first). Use Window::ShowState::hidden to style programmatically
   * before displaying
   * @return Window* weak pointer to window (application object owns memory)
   *
   * @throw std::exception on failure
   */
  Window* addWindow(const char* id,
                    Size size = defaultWindowSize,
                    Window::ShowState showState =
                        Window::ShowState::restore) noexcept(false) {
    YAUL_EXCEPTION_WRAPPER_THROW(Window*, apiAddWindow, id, size, showState);
  }

  /**
   * @brief Blocking call returned once all windows have closed
   *
   */
  void waitForAllWindowsToClose() noexcept;

 private:
  Window* apiAddWindow(const char* id,
                       Size size,
                       Window::ShowState showState,
                       Result* r) noexcept;
};

}  // namespace yaul

#endif /* _YAUL_APPLICATION_HPP_ */