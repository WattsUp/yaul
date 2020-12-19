#ifndef _YAUL_IMPL_APPLICATION_HPP_
#define _YAUL_IMPL_APPLICATION_HPP_

#include <yaul/application.hpp>
#include <yaul/window.hpp>

#include "string.hpp"

namespace yaul::impl {

class Application {
 public:
  /**
   * @brief Construct a new Application::Impl object
   * Implementation for pimpl design
   *
   * @param argc command line argument count
   * @param argv command line arguments
   * @param settings of the application
   */
  Application(int argc,
              char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
              const ApplicationSettings& settings) noexcept;

  yaul::Window addWindow(const string& id);

 private:
  std::list<yaul::Window*> windows{};
};

}  // namespace yaul::impl

#endif /* _YAUL_IMPL_APPLICATION_HPP_ */