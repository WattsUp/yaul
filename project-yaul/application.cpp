#include "application_impl.hpp"

#include "common/logger.hpp"

#include <condition_variable>

namespace yaul {

Application::Impl::Impl(int argc,
                        char** argv,
                        const ApplicationSettings& settings) noexcept {
  Logger::instance().setLogger(settings.logger);

  for (int i = 0; i < argc; ++i)
    Logger::instance().log(LogLevel::debug, argv[i]);

  doRender = !settings.customRenderLoop;
  running  = true;
  thread   = std::make_unique<std::thread>(&Application::Impl::loop, this);

  // Wait for loop to start up
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, [this] { return static_cast<bool>(threadReady); });
}

Application::Impl::~Impl() noexcept {
  stop();
}

Window Application::Impl::addWindow(
    const char* id,
    Size size,
    Window::ShowState showState) noexcept(false) {
  // Send new window info to loop
  NewWindowInfo windowInfo;
  windowInfo.size = size;
  windowInfo.id   = string(id);

  {
    // Check for window already created
    std::unique_lock<std::mutex> lock(mutex);
    auto search = windows.find(windowInfo.id);
    if (search != windows.end())
      return search->second;

    newWindowInfo = &windowInfo;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Send new window message
    if (::PostThreadMessageW(::GetThreadId(thread->native_handle()),
                             YAUL_WM_NEW_WINDOW, 0, 0) == 0)
      throw std::runtime_error("Failed to post new window message");

#endif /* WIN32 */

    // Wait for loop to create window
    // windowInfo.complete is true once consumed
    cv.wait(lock, [&windowInfo] { return windowInfo.complete; });
  }

  if (windowInfo.result.failed())
    throw std::runtime_error(static_cast<char*>(windowInfo.result));

  // Lookup and apply any styling from XML/CSS
  windowInfo.createdWindow.setTitle(id);         // TODO (WattsUp) get title
  windowInfo.createdWindow.setBorderless(true);  // Yaul draws custom a menubar

  windowInfo.createdWindow.setShowState(showState);

  return windowInfo.createdWindow;
}

void Application::Impl::waitForAllWindowsToClose() noexcept {
  if (windows.empty() || !running)
    return;

  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, [this]() { return windows.empty(); });
}

/******************************** Public Class ********************************/

Application::Application(int argc,
                         char** argv,
                         const ApplicationSettings& settings) noexcept
    : Object(std::make_unique<Impl>(argc, argv, settings)) {}

YAUL_IMPL_DESTRUCT(Application);
YAUL_IMPL_MOVE(Application, Object);

Window Application::apiAddWindow(const char* id,
                                 Size size,
                                 Window::ShowState showState,
                                 Result* const r) noexcept {
  YAUL_EXCEPTION_WRAPPER_CATCH(
      return impl<Impl>()->addWindow(id, size, showState));
  return Window();
}

void Application::waitForAllWindowsToClose() noexcept {
  impl<Impl>()->waitForAllWindowsToClose();
}

}  // namespace yaul