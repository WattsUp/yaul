#include "monitor_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

/******************************** Public Class ********************************/

Monitor::Monitor() noexcept : Object(nullptr) {}

YAUL_IMPL_DESTRUCT_MOVE_COPY(Monitor, Object);

std::list<Monitor> Monitor::enumerate() noexcept {
  std::list<Monitor> monitors;
  for (auto& monitorImpl : Impl::enumerate()) {
    Monitor monitor;
    monitor.setImpl(std::make_unique<Impl>(monitorImpl));
    monitors.emplace_back(monitor);
  }
  return monitors;
}

const char* Monitor::getName() const noexcept {
  return impl<Impl>()->getName().c_str();
}

Size Monitor::getSize() const noexcept {
  return impl<Impl>()->getSize();
}

float Monitor::getDPMM() const noexcept {
  return impl<Impl>()->getDPMM();
}

Position Monitor::getPosition() const noexcept {
  return impl<Impl>()->getPosition();
}

bool Monitor::isPrimary() const noexcept {
  return impl<Impl>()->isPrimary();
}

}  // namespace yaul