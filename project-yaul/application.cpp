#include <yaul/application.hpp>

#include "impl/application.hpp"
#include "logger.hpp"

namespace yaul {

Application::Application(
    int argc,
    char* argv[],  // NOLINT (cppcoreguidelines-avoid-c-arrays)
    const ApplicationSettings& settings) noexcept
    : pImpl(new impl::Application(argc, argv, settings)) {}

Application::~Application() noexcept {
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
    : pImpl(new impl::Application(*o.pImpl)) {}

Application& Application::operator=(const Application& o) noexcept {
  if (this != &o) {
    // NOLINTNEXTLINE (cppcoreguidelines-owning-memory)
    pImpl = new impl::Application(*o.pImpl);
  }
  return *this;
}

Window Application::apiAddWindow(const char* id, Result& r) noexcept {
  YAUL_EXCEPTION_WRAPPER_CATCH(return pImpl->addWindow(string(id)), r);
  return Window();
}

}  // namespace yaul