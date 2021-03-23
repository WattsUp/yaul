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
  Window window;
  string idString(id);

  {
    // Check for window already created
    std::unique_lock<std::mutex> lock(mutex);
    auto search = windows.find(idString);
    if (search != windows.end())
      return search->second;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Need event loop thread to create window

    // Send new window info to loop
    NewWindowInfo windowInfo;
    windowInfo.size = size;
    windowInfo.id   = idString;

    newWindowInfo = &windowInfo;

    // Send new window message
    if (::PostThreadMessageW(::GetThreadId(thread->native_handle()),
                             YAUL_WM_NEW_WINDOW, 0, 0) == 0)
      throw std::runtime_error("Failed to post new window message");
    // Wait for loop to create window
    // windowInfo.complete is true once consumed
    cv.wait(lock, [&windowInfo] { return windowInfo.complete; });

    if (windowInfo.result.failed())
      throw std::runtime_error(static_cast<char*>(windowInfo.result));

    window = windowInfo.createdWindow;
#else /* !WIN32 */
    // Create window in this thread, no need to be created in event loop
    window = Window(size, "", Window::ShowState::hidden);
    windows.emplace(idString, window);
#endif /* WIN32 */
  }

  // Lookup and apply any styling from XML/CSS
  window.setTitle(id);         // TODO (WattsUp) get title
  window.setBorderless(true);  // Yaul draws custom a menubar

  // setBorderless preserves size but there is a race condition with async
  // SetWindowPos that could fail and result in an improper size
  window.setSize(size);

  window.setShowState(showState);

  return window;
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