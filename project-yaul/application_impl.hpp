#ifndef _YAUL_APPLICATION_IMPL_HPP_
#define _YAUL_APPLICATION_IMPL_HPP_

#include <yaul/application.hpp>

#include "object_impl.hpp"

#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <Windows.h>

#define YAUL_WM_NEW_WINDOW (WM_USER + 0)
#define YAUL_WM_STOP_LOOP (WM_USER + 1)

#endif /* WIN32 */

namespace yaul {

class Application::Impl final : public Object::Impl {
 public:
  /**
   * @brief Construct a yaul application
   *
   * @param argc command line argument count
   * @param argv command line arguments
   * @param settings of the application
   */
  Impl(int argc, char* argv[], const ApplicationSettings& settings) noexcept;

  YAUL_DEFINE_DESTRUCT(Impl);
  YAUL_NO_COPY(Impl);

  /**
   * @brief Add a window to the application
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
  Window* addWindow(
      const char* id,
      Size size                   = defaultWindowSize,
      Window::ShowState showState = Window::ShowState::restore) noexcept(false);

  /**
   * @brief Blocking call returned once all windows have closed
   *
   */
  void waitForAllWindowsToClose() noexcept;

 private:
  /**
   * @brief Application loop that creates and polls windows, dispatches events,
   * and renders (if enabled)
   *
   */
  void loop() noexcept;

  /**
   * @brief Stop the application loop
   *
   */
  void stop() noexcept;

  struct NewWindowInfo {
    Size size;
    Window::ShowState showState = Window::ShowState::restore;
    const char* id              = nullptr;
    Window* createdWindow       = nullptr;
    Result result;
    bool complete = false;
  };

  std::mutex mutex;
  std::condition_variable cv;

  NewWindowInfo* newWindowInfo = nullptr;

  std::list<std::unique_ptr<Window>> windows{};

  bool doRender                       = true;
  std::atomic<bool> running           = false;
  std::atomic<bool> threadReady       = false;
  std::unique_ptr<std::thread> thread = nullptr;
};

}  // namespace yaul

#endif /* _YAUL_APPLICATION_IMPL_HPP_ */