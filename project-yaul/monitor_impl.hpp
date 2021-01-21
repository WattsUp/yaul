#ifndef YAUL_MONITOR_IMPL_HPP
#define YAUL_MONITOR_IMPL_HPP

#include <yaul/monitor.hpp>

#include "common/string.hpp"
#include "object_impl.hpp"

#include <list>

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
   * @brief Get the DPI (dots/pixels per inch) of the monitor
   *
   * @return int in DPI, defaultDPI if unknown
   */
  [[nodiscard]] inline int getDPI() const noexcept { return dpi; }

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
   * @brief Construct a new Monitor implementation by by queuering display
   * settings with the specified display device name
   *
   * @param deviceName to query display settings
   * @param friendlyName of the monitor
   * @param primary true indicates this monitor is the primary device
   */
  explicit Impl(wchar_t* deviceName,
                wchar_t* friendlyName,
                bool primary = false) noexcept;
#endif /* WIN32 */

  string name;
  Size size;
  Position position;
  int dpi      = defaultDPI;
  bool primary = false;
};

}  // namespace yaul

#endif /* _YAUL_MONITOR_IMPL_HPP_ */