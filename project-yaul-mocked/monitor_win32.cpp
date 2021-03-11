#include "monitor_mocked.hpp"

BOOL EnumDisplayDevicesW_mocked(LPCWSTR lpDevice,
                                DWORD iDevNum,
                                PDISPLAY_DEVICEW lpDisplayDevice,
                                DWORD dwFlags) {
  if (lpDisplayDevice->cb != sizeof(DISPLAY_DEVICEW))
    return FALSE;

  if (lpDevice != nullptr) {
    if (iDevNum != 0)
      return FALSE;

    auto itr = MonitorMock::adapters.begin();
    size_t i = 0;
    while (itr != MonitorMock::adapters.end() &&
           ::wcscmp(lpDevice, itr->adapterName) != 0) {
      ++itr;
      ++i;
    }
    if (itr == MonitorMock::adapters.end())
      return FALSE;

    ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceName),
               sizeof(lpDisplayDevice->DeviceName) / sizeof(WCHAR),
               itr->monitorName);
    ::MultiByteToWideChar(
        CP_UTF8, 0, MonitorMock::monitors.at(i).name, -1,
        static_cast<wchar_t*>(lpDisplayDevice->DeviceString),
        sizeof(lpDisplayDevice->DeviceString) / sizeof(WCHAR));
    ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceID),
               sizeof(lpDisplayDevice->DeviceID) / sizeof(WCHAR),
               itr->monitorID);
    ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceKey),
               sizeof(lpDisplayDevice->DeviceKey) / sizeof(WCHAR),
               itr->monitorKey);

    lpDisplayDevice->StateFlags =
        DISPLAY_DEVICE_ACTIVE | DISPLAY_DEVICE_ATTACHED;
    return TRUE;
  }

  if (iDevNum >= MonitorMock::count)
    return FALSE;

  if (dwFlags != 0)
    return FALSE;  // TODO (WattsUp) handle non-zero flags:
                   // EDD_GET_DEVICE_INTERFACE_NAME

  const auto& monitor = MonitorMock::adapters.at(iDevNum);
  ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceName),
             sizeof(lpDisplayDevice->DeviceName) / sizeof(WCHAR),
             monitor.adapterName);
  ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceString),
             sizeof(lpDisplayDevice->DeviceString) / sizeof(WCHAR),
             monitor.adapterString);
  ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceID),
             sizeof(lpDisplayDevice->DeviceID) / sizeof(WCHAR),
             monitor.adapterID);
  ::wcscpy_s(static_cast<wchar_t*>(lpDisplayDevice->DeviceKey),
             sizeof(lpDisplayDevice->DeviceKey) / sizeof(WCHAR),
             monitor.adapterKey);

  lpDisplayDevice->StateFlags =
      DISPLAY_DEVICE_ATTACHED_TO_DESKTOP |
      (iDevNum == MonitorMock::primary ? DISPLAY_DEVICE_PRIMARY_DEVICE : 0);

  return TRUE;
}

BOOL EnumDisplaySettingsW_mocked(LPCWSTR lpszDeviceName,
                                 DWORD iModeNum,
                                 DEVMODEW* lpDevMode) {
  if (lpDevMode->dmSize != sizeof(DEVMODEW))
    return FALSE;

  if (lpszDeviceName == nullptr)
    return FALSE;

  if (iModeNum != ENUM_CURRENT_SETTINGS)
    return FALSE;

  auto itr = MonitorMock::adapters.begin();
  size_t i = 0;
  while (itr != MonitorMock::adapters.end() &&
         ::wcscmp(lpszDeviceName, itr->adapterName) != 0) {
    ++itr;
    ++i;
  }
  if (itr == MonitorMock::adapters.end())
    return FALSE;

  const auto& monitor = MonitorMock::monitors.at(i);

  lpDevMode->dmPelsWidth  = static_cast<DWORD>(monitor.rect.width);
  lpDevMode->dmPelsHeight = static_cast<DWORD>(monitor.rect.height);

  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access)
  lpDevMode->dmPosition.x = static_cast<LONG>(monitor.rect.x);
  // NOLINTNEXTLINE (cppcoreguidelines-pro-type-union-access)
  lpDevMode->dmPosition.y = static_cast<LONG>(monitor.rect.y);

  lpDevMode->dmLogPixels =
      static_cast<WORD>(monitor.dpmm * ::yaul::units::mmPerInch);

  lpDevMode->dmFields =
      DM_POSITION | DM_LOGPIXELS | DM_PELSWIDTH | DM_PELSHEIGHT;

  return TRUE;
}