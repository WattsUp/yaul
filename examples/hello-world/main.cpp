#include "yaul/yaul.h"

#include <cstdio>
#include <exception>

/**
 * @brief Main entry point for program
 *
 * @param argc count of arguments
 * @param argv array of arguments
 * @return int zero on success, non-zero on failure
 */
int main(int argc, char* argv[]) {
  ::yaul::configureLogging("log.log");
  for (int i = 0; i < argc; ++i) {
    ::yaul::log(argv[i]);
  }
  ::yaul::log("!🌍 olleH");
  // log("Starting hello world application");
  // ::yaul::initialize("Hello world application");
  // ::yaul::Window window = ::yaul::addWindow("unique window id");

  // ::yaul::String dialog = window.addString("dialog", "");

  // for (const char& c : "Hello world!") {
  //   dialog += c;
  //   ::yaul::sleep(::yaul::Time("1s"));
  // }

  // window.close();
  // ::yaul::exit();
  return 0;
}