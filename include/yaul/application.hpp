#ifndef _YAUL_APPLICATION_HPP_
#define _YAUL_APPLICATION_HPP_

#include "common.hpp"
#include "window.hpp"

namespace yaul {

/**
 * @brief Application settings
 *
 * @param logger function to direct log statements to
 */
struct ApplicationSettings {
  logger_t logger = nullptr;
};

class YAUL_API Application {
 public:
  /**
   * @brief Construct a yaul application
   *
   * @param argc command line argument count
   * @param argv command line arguments
   * @param settings of the application
   */
  Application(int argc,
              char* argv[],
              const ApplicationSettings& settings) noexcept;
  ~Application() noexcept;

  Application(Application&& o) noexcept;
  Application& operator=(Application&& o) noexcept;

  Application(const Application& o) noexcept;
  Application& operator=(const Application& o) noexcept;

  /**
   * @brief Add a window to the application
   *
   * @param id of the window to refer to in XML/CSS
   * @return Window object handle
   *
   * @throw std::exception on failure
   */
  Window addWindow(const char* id) noexcept(false) {
    YAUL_EXCEPTION_WRAPPER_THROW(Window, apiAddWindow, id);
  }

 private:
  Window apiAddWindow(const char* id, Result& r) noexcept;

  class Impl;
  Impl* pImpl = nullptr;
};

}  // namespace yaul

#endif /* _YAUL_APPLICATION_HPP_ */