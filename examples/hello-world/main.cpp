#include <yaul/yaul.hpp>

#include <fstream>

std::ofstream logFile;

/**
 * @brief Log a message from yaul to the console
 *
 * @param level
 * @param msg
 */
void logger(::yaul::LogLevel level, const char* msg) noexcept {
  try {
    if (!logFile.is_open()) {
      return;
    }
    switch (level) {
      case ::yaul::LogLevel::debug:
        logFile << "[ DEBUG  ] ";
        break;
      case ::yaul::LogLevel::info:
        logFile << "[  INFO  ] ";
        break;
      case ::yaul::LogLevel::warning:
        logFile << "[WARNING ] ";
        break;
      case ::yaul::LogLevel::error:
        logFile << "[ ERROR  ] ";
        break;
      case ::yaul::LogLevel::critical:
        logFile << "[CRITICAL] ";
        break;
      case ::yaul::LogLevel::off:
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
    logFile.open("log.log", std::ios::app);
  } catch (std::exception& e) {
    puts("Failed to open log file\n");
  }
  ::yaul::ApplicationSettings settings;
  settings.logger = logger;
  auto app        = ::yaul::Application(argc, argv, settings);

  try {
    ::yaul::Window window = app.addWindow("unique window id");
    window.show();
  } catch (const std::exception& e) {
    logger(yaul::LogLevel::critical, e.what());
  }

  // ::yaul::String dialog = window.addString("dialog", "");

  // for (const char& c : "Hello world!") {
  //   dialog += c;
  //   ::yaul::sleep(::yaul::Time("1s"));
  // }

  // window.close();
  // ::yaul::exit();
  try {
    logFile.close();
  } catch (std::exception& e) {
    puts("Failed to close log file\n");
  }
  return 0;
}