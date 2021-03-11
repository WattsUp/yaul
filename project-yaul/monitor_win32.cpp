#include "monitor_impl.hpp"

#include "common/logger.hpp"

#ifdef YAUL_MOCKED
#include "monitor_mocked.hpp"
#endif

namespace yaul {

std::list<Monitor::Impl> Monitor::Impl::enumerate() noexcept {
  std::list<Monitor::Impl> monitors;
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
  return monitors;
}

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
  position.x = deviceMode.dmPosition.x;
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access)
  position.y = deviceMode.dmPosition.y;

  dpmm = static_cast<float>(deviceMode.dmLogPixels) / units::mmPerInch;
}

}  // namespace yaul