#include "application.hpp"

#include "logger.hpp"

namespace yaul::impl {

Application::Application(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings) noexcept {
  Logger::instance().setLogger(settings.logger);

  for (int i = 0; i < argc; ++i) {
    Logger::instance().log(LogLevel::debug, argv[i]);
  }
  // printf("Logger 0x%08X\n", (unsigned int)settings.logger);
}

yaul::Window Application::addWindow(const string& id) {
  Logger::instance().log(LogLevel::debug, "ID: " + id);
  // throw std::runtime_error("Error adding window");
  yaul::Window window(id.c_str());
  windows.push_back(&window);
  return window;
}

}  // namespace yaul::impl