#include "yaul/application.hpp"

#include "logger.hpp"

namespace yaul {

class Application::Impl {
 public:
  // NOLINTNEXTLINE (cppcoreguidelines-avoid-c-arrays)
  Impl(int argc, char* argv[], const ApplicationSettings& settings);
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
  // printf("Application name: %s\n", settings.applicationName);
  // printf("Logger 0x%08X\n", (unsigned int)settings.logger);
}

/******************************* pimpl Wrapper ********************************/

Application::Application(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings)
    : pImpl(std::make_unique<Impl>(argc, argv, settings)) {}

Application::~Application()                      = default;
Application::Application(Application&&) noexcept = default;
Application& Application::operator=(Application&&) noexcept = default;

Application::Application(const Application& app)
    : pImpl(std::make_unique<Impl>(*app.pImpl)) {}

Application& Application::operator=(const Application& app) {
  if (this != &app) {
    pImpl = std::make_unique<Impl>(*app.pImpl);
  }
  return *this;
}
}  // namespace yaul