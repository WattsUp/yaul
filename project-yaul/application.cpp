#include "application_impl.hpp"

#include "common/logger.hpp"

#include <condition_variable>

namespace yaul {

Application::Impl::Impl(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings) noexcept {
  Logger::instance().setLogger(settings.logger);

  for (int i = 0; i < argc; ++i) {
    Logger::instance().log(LogLevel::debug, argv[i]);
  }
  doRender = !settings.customRenderLoop;
  running  = true;
  thread   = std::make_unique<std::thread>(&Application::Impl::loop, this);
}

Application::Impl::~Impl() noexcept {
  stop();
}

Window* Application::Impl::addWindow(
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
    if (search != windows.end()) {
      return search->second.get();
    }

    // Wait for loop to become a message processing thread
    cv.wait(lock, [this] { return static_cast<bool>(threadReady); });
    newWindowInfo = &windowInfo;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Send new window message
    if (::PostThreadMessageW(::GetThreadId(thread->native_handle()),
                             YAUL_WM_NEW_WINDOW, 0, 0) == 0) {
      throw std::exception("Failed to post new window message");
    }
#endif /* WIN32 */

    // Wait for loop to create window
    // windowInfo.complete is true once consumed
    cv.wait(lock, [&windowInfo] { return windowInfo.complete; });
  }

  if (windowInfo.result.failed()) {
    throw std::exception(windowInfo.result);
  }

  // Lookup and apply any styling from XML/CSS
  windowInfo.createdWindow->setTitle(id);         // TODO (WattsUp) get title
  windowInfo.createdWindow->setBorderless(true);  // Yaul draws custom a menubar

  windowInfo.createdWindow->setShowState(showState);

  return windowInfo.createdWindow;
}

void Application::Impl::waitForAllWindowsToClose() noexcept {
  if (windows.empty() || !running) {
    return;
  }
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, [this]() { return windows.empty(); });
}

/******************************** Public Class ********************************/

Application::Application(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings) noexcept
    : Object(*new Application::Impl(argc, argv, settings)) {}

YAUL_IMPL_DESTRUCT(Application);
YAUL_IMPL_MOVE(Application, Object);

Window* Application::apiAddWindow(const char* id,
                                  Size size,
                                  Window::ShowState showState,
                                  Result& r) noexcept {
  YAUL_EXCEPTION_WRAPPER_CATCH(
      return dynamic_cast<Application::Impl*>(pImpl)->addWindow(id, size,
                                                                showState),
             r);
  return nullptr;
}

void Application::waitForAllWindowsToClose() noexcept {
  dynamic_cast<Application::Impl*>(pImpl)->waitForAllWindowsToClose();
}

}  // namespace yaul