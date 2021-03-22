#ifndef YAUL_MONITOR_MOCKED_HPP
#define YAUL_MONITOR_MOCKED_HPP

#include <array>
#include <yaul/dimensions.hpp>

namespace MonitorMock {
constexpr size_t count   = 3;
constexpr size_t primary = 1;

struct MonitorInfo {
  const char* name;
  const ::yaul::Rectangle rect;
  const float dpmm;
  const bool primary;
};

}  // namespace MonitorMock

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
namespace MonitorMock {
const std::array<MonitorInfo, count> monitors = {
    MonitorInfo{"Dell P2213 (Digital - DisplayPort)",
                {0, 0, 1920, 1080},
                96 / ::yaul::units::mmPerInch,
                false},
    MonitorInfo{"Dell P2213 (Digital - DisplayPort)",
                {1920, 0, 19200, 10800},
                3023 / ::yaul::units::mmPerInch,
                true},
    MonitorInfo{"Dell P2213 (Digital - HDMI)",
                {100, -5678, 1234, 5678},
                48 / ::yaul::units::mmPerInch,
                false},
};

struct AdapterInfo {
  const wchar_t* adapterName;
  const wchar_t* adapterString;
  const wchar_t* adapterID;
  const wchar_t* adapterKey;

  const wchar_t* monitorName;
  // const wchar_t* monitorString; // Use MonitorInfo
  const wchar_t* monitorID;
  const wchar_t* monitorKey;
};

// clang-format off
const std::array<AdapterInfo, count> adapters = {
    AdapterInfo{
        L"\\\\.\\DISPLAY0",
        L"NVIDIA NVS 310",
        L"PCI\\VEN_10DE&DEV107D&SUBSYS_094E10DE&REV_A1",
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{7a961182-34c4-40dc-9211-2231b75ccd33}\\0000",

        L"\\\\.\\DISPLAY0\\Monitor0",
        // L"Dell P2213 (Digital - DisplayPort)",
        L"MONITOR\\DELF043\\{80a836ea-0446-460b-b397-2899a40c84a3}\\0001",
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Class\\{80a836ea-0446-460b-b397-2899a40c84a3}\\0001"},
    AdapterInfo{
        L"\\\\.\\DISPLAY1",
        L"NVIDIA NVS 310",
        L"PCI\\VEN_10DE&DEV107D&SUBSYS_094E10DE&REV_A1",
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{7a961182-34c4-40dc-9211-2231b75ccd33}\\0001",

        L"\\\\.\\DISPLAY1\\Monitor0",
        // L"Dell P2213 (Digital - DisplayPort)",
        L"MONITOR\\DELF043\\{80a836ea-0446-460b-b397-2899a40c84a3}\\0002",
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Class\\{80a836ea-0446-460b-b397-2899a40c84a3}\\0002"},
    AdapterInfo{
        L"\\\\.\\DISPLAY3",
        L"NVIDIA RTX 3070",
        L"PCI\\VEN_10DE&DEV2484&SUBSYS_87B81043&REV_A1",
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Video\\{c358b121-0294-45d4-82f1-e26535ec8932}\\0000",

        L"\\\\.\\DISPLAY3\\Monitor0",
        // L"Dell P2213 (Digital - DisplayPort)",
        L"MONITOR\\DELF043\\{a7456a0e-bc97-4229-b42c-fc85793538a9}\\0001",
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Control\\Class\\{a7456a0e-bc97-4229-b42c-fc85793538a9}\\0001"}
    };
// clang-format on
}  // namespace MonitorMock

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef EnumDisplayDevicesW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define EnumDisplayDevicesW EnumDisplayDevicesW_mocked
BOOL EnumDisplayDevicesW_mocked(LPCWSTR lpDevice,
                                DWORD iDevNum,
                                PDISPLAY_DEVICEW lpDisplayDevice,
                                DWORD dwFlags);

#undef EnumDisplaySettingsW
// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define EnumDisplaySettingsW EnumDisplaySettingsW_mocked
BOOL EnumDisplaySettingsW_mocked(LPCWSTR lpszDeviceName,
                                 DWORD iModeNum,
                                 DEVMODEW* lpDevMode);

#elif defined(__linux) || defined(__linux__)
#endif /* WIN32, __linux__ */

#endif /* YAUL_MONITOR_MOCKED_HPP */