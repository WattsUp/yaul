#include "yaul/application.hpp"

#include "logger.hpp"
#include "string.hpp"

namespace yaul {

class Application::Impl {
 public:
  // NOLINTNEXTLINE (cppcoreguidelines-avoid-c-arrays)
  Impl(int argc, char* argv[], const ApplicationSettings& settings);

  Window addWindow(const string& id);

 private:
  std::list<Window*> windows;
};

/**
 * @brief Construct a new Application::Impl object
 * Implementation for pimpl design
 *
 * @param argc command line argument count
 * @param argv command line arguments
 * @param settings of the application
 */
Application::Impl::Impl(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings) {
  Logger::instance().setLogger(settings.logger);

  for (int i = 0; i < argc; ++i) {
    Logger::instance().log(LogLevel::debug, argv[i]);
  }
  // printf("Logger 0x%08X\n", (unsigned int)settings.logger);
}

Window Application::Impl::addWindow(const string& id) {
  Logger::instance().log(LogLevel::debug, "ID: " + id);
  // throw std::runtime_error("Error adding window");
  Window window(id.c_str());
  windows.push_back(&window);
  return window;
}

/******************************* pimpl Wrapper ********************************/

Application::Application(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings) noexcept
    : pImpl(new Impl(argc, argv, settings)) {}

Application::~Application() {
  delete pImpl;
}

Application::Application(Application&& o) noexcept {
  pImpl   = o.pImpl;
  o.pImpl = nullptr;
}

Application& Application::operator=(Application&& o) noexcept {
  if (&o == this) {
    return *this;
  }
  delete pImpl;
  pImpl   = o.pImpl;
  o.pImpl = nullptr;
  return *this;
}

Application::Application(const Application& o) noexcept
    : pImpl(new Impl(*o.pImpl)) {}

Application& Application::operator=(const Application& o) noexcept {
  if (this != &o) {
    pImpl = new Impl(*o.pImpl);  // NOLINT (cppcoreguidelines-owning-memory)
  }
  return *this;
}

Window Application::apiAddWindow(const char* id, Result& r) noexcept {
  YAUL_EXCEPTION_WRAPPER_CATCH(return pImpl->addWindow(string(id)), r);
  return Window();
}

}  // namespace yaul