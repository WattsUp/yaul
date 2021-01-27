#include <yaul/yaul.hpp>

#include <fstream>
#include <thread>

// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
std::ofstream logFile;

/**
 * @brief Log a message from yaul to the console
 *
 * @param level
 * @param msg
 */
void logger(yaul::LogLevel level, const char* msg) noexcept {
  try {
    if (!logFile.is_open())
      return;
    switch (level) {
      case yaul::LogLevel::debug:
        logFile << "[ DEBUG  ] ";
        break;
      case yaul::LogLevel::info:
        logFile << "[  INFO  ] ";
        break;
      case yaul::LogLevel::warning:
        logFile << "[WARNING ] ";
        break;
      case yaul::LogLevel::error:
        logFile << "[ ERROR  ] ";
        break;
      case yaul::LogLevel::critical:
        logFile << "[CRITICAL] ";
        break;
      case yaul::LogLevel::off:
        return;
    }
    logFile << msg << std::endl;
  } catch (const std::exception& e) {
    puts("Failed to log a message to log file\n");
    puts(e.what());
  }
}

/**
 * @brief Main entry point for program
 *
 * @param argc count of arguments
 * @param argv array of arguments
 * @return int zero on success, non-zero on failure
 */
int main(int argc, char* argv[]) {
  try {
    logFile.open("hello-world.log", std::ios::app);
  } catch (std::exception& e) {
    puts("Failed to open log file\n");
    return -1;
  }
  yaul::ApplicationSettings settings;
  settings.logger = logger;
  auto app        = yaul::Application(argc, argv, settings);

  auto monitors = yaul::Monitor::enumerate();
  for (const auto& monitor : monitors)
    logger(yaul::LogLevel::debug, monitor.getName());

  try {
    auto window = app.addWindow(u8"unique window id");
    window.setTitle(u8"🌍Hello World🌍");
    window.setPosition({128, 256}, &monitors.back());
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // window.setFullscreen(true);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // // window.setFullscreen(true);
    // window.setFullscreen(true, &monitors.front());
    // delete window; corruption

  } catch (const ::std::exception& e) {
    logger(yaul::LogLevel::critical, e.what());
    return -1;
  }
  app.waitForAllWindowsToClose();

  // yaul::String dialog = window.addString("dialog", "");

  // for (const char& c : "Hello world!") {
  //   dialog += c;
  //   yaul::sleep(yaul::Time("1s"));
  // }

  // window.close();
  // yaul::exit();
  try {
    logFile.close();
  } catch (std::exception& e) {
    puts("Failed to close log file\n");
    return -1;
  }
  return 0;
}