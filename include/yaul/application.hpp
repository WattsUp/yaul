#ifndef _YAUL_APPLICATION_HPP_
#define _YAUL_APPLICATION_HPP_

#include "api.hpp"
#include "log.hpp"

#include <memory>

namespace yaul {

/**
 * @brief Application settings
 *
 * @param applicationName name to register with window manager
 * @param logger function to direct log statements to
 */
struct ApplicationSettings {
  const char* applicationName = "Yaul application";
  logger_t logger             = nullptr;
};

class Application {
 public:
  /**
   * @brief Construct a yaul application
   *
   * @param argc command line argument count
   * @param argv command line arguments
   * @param settings of the application
   */
  YAUL_API Application(int argc,
                       char* argv[],
                       const ApplicationSettings& settings);
  YAUL_API ~Application();

  YAUL_API Application(Application&& app) noexcept;
  YAUL_API Application& operator=(Application&& app) noexcept;

  YAUL_API Application(const Application& app);
  YAUL_API Application& operator=(const Application& app);

 private:
  class Impl;
  std::unique_ptr<Impl> pImpl;
};

}  // namespace yaul

#endif /* _YAUL_APPLICATION_HPP_ */