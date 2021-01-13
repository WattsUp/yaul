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