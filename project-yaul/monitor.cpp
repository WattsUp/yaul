#include "monitor_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

/**
 * @brief Enumerate the monitors available on the system
 *
 * @return std::list<Monitor>
 */
std::list<Monitor::Impl> Monitor::Impl::enumerate() noexcept {
  std::list<Monitor::Impl> monitors;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  for (DWORD i = 0;; ++i) {
    DISPLAY_DEVICEW display;
    ZeroMemory(&display, sizeof(display));
    display.cb = sizeof(display);

    if (::EnumDisplayDevicesW(nullptr, i, &display, 0) == FALSE)
      break;

    if ((display.StateFlags & DISPLAY_DEVICE_ACTIVE) == 0)
      continue;

    bool primary = (display.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0;

    // Get the friendly name of the first monitor, duplicated monitors show up
    // as a single display
    DISPLAY_DEVICEW monitor;
    ZeroMemory(&monitor, sizeof(monitor));
    monitor.cb = sizeof(monitor);
    if (::EnumDisplayDevicesW(static_cast<wchar_t*>(display.DeviceName), 0,
                              &monitor, 0) == FALSE)
      break;

    monitors.emplace_back(
        Monitor::Impl(static_cast<wchar_t*>(display.DeviceName),
                      static_cast<wchar_t*>(monitor.DeviceString), primary));
  }
#endif /* WIN32 */
  return monitors;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
Monitor::Impl::Impl(wchar_t* deviceName,
                    wchar_t* friendlyName,
                    bool primary) noexcept {
  this->primary = primary;
  name          = WideChar::rev(friendlyName);

  DEVMODEW deviceMode;
  ZeroMemory(&deviceMode, sizeof(deviceMode));
  deviceMode.dmSize = sizeof(deviceMode);

  if (::EnumDisplaySettingsW(deviceName, ENUM_CURRENT_SETTINGS, &deviceMode) ==
      FALSE)
    return;

  size.width  = static_cast<int>(deviceMode.dmPelsWidth);
  size.height = static_cast<int>(deviceMode.dmPelsHeight);

  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access)
  position.left = deviceMode.dmPosition.x;
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access)
  position.top = deviceMode.dmPosition.y;

  dpi = deviceMode.dmLogPixels;
}
#endif /* WIN32 */

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

int Monitor::getDPI() const noexcept {
  return impl<Impl>()->getDPI();
}
Position Monitor::getPosition() const noexcept {
  return impl<Impl>()->getPosition();
}

bool Monitor::isPrimary() const noexcept {
  return impl<Impl>()->isPrimary();
}

}  // namespace yaul