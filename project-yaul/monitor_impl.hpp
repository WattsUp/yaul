#ifndef YAUL_MONITOR_IMPL_HPP
#define YAUL_MONITOR_IMPL_HPP

#include <yaul/monitor.hpp>

#include "common/string.hpp"
#include "object_impl.hpp"

#include <list>

#if defined(__linux) || defined(__linux__)
#include "xcb.hpp"
#endif /* __linux__ */

namespace yaul {

class Monitor::Impl : public Object::Impl {
 public:
  /**
   * @brief Enumerate the monitors available on the system
   *
   * @return std::list<Monitor>
   */
  static std::list<Monitor::Impl> enumerate() noexcept;

  /**
   * @brief Get the name of the monitor, usually manufacturer and model number
   *
   * @return const string& name
   */
  [[nodiscard]] inline const string& getName() const noexcept { return name; }

  /**
   * @brief Get the size of the monitor
   *
   * @return Size in pixels
   */
  [[nodiscard]] inline Size getSize() const noexcept { return size; }

  /**
   * @brief Get the DPMM (dots/pixels per millimeter) of the monitor
   *
   * @return float in DPMM, defaultDPMM if unknown
   */
  [[nodiscard]] inline float getDPMM() const noexcept { return dpmm; }

  /**
   * @brief Get the position of the monitor relative to the primary. Distance
   * from this monitor's top left pixel to the primary monitor's top left pixel.
   *
   * @return Position in pixels
   */
  [[nodiscard]] inline Position getPosition() const noexcept {
    return position;
  }

  // ColorSpace getColorSpace something about bit depth, gamma, LUTs, etc

  /**
   * @brief Check if this monitor is the primary monitor
   *
   * @return true if this monitor is the primary monitor, false otherwise
   */
  [[nodiscard]] inline bool isPrimary() const noexcept { return primary; }

 private:
  /**
   * @brief Construct a new Monitor implementation object with default values
   *
   */
  Impl() noexcept = default;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  /**
   * @brief Construct a new Monitor implementation by queuering display
   * settings with the specified display device name
   *
   * @param deviceName to query display settings
   * @param friendlyName of the monitor
   * @param primary true indicates this monitor is the primary device
   */
  explicit Impl(wchar_t* deviceName,
                wchar_t* friendlyName,
                bool primary = false) noexcept;
#elif defined(__linux) || defined(__linux__)
  /**
   * @brief Construct a new Monitor implementation by queruering monitor info
   * and xcb connection
   *
   * @param monitorInfo xcb struct containing size, position, DPMM info, etc.
   */
  explicit Impl(xcb_randr_monitor_info_t* monitorInfo) noexcept;
#endif /* WIN32, __linux__ */

  string name;
  Size size;
  Position position;
  float dpmm   = defaultDPMM;
  bool primary = false;
};

}  // namespace yaul

#endif /* YAUL_MONITOR_IMPL_HPP */