#ifndef YAUL_MONITOR_HPP
#define YAUL_MONITOR_HPP

#include <list>

#include <yaul/dimensions.hpp>
#include <yaul/object.hpp>

#if defined(__linux) || defined(__linux__)
#include <xcb/randr.h>
#endif /* __linux__ */

namespace yaul {

class YAUL_API Monitor final : public Object {
 public:
  static constexpr float defaultDPMM = 96.0F / units::mmPerInch;

  /**
   * @brief Enumerate the monitors available on the system
   *
   * @return std::list<Monitor>
   */
  static std::list<Monitor> enumerate() noexcept;

  YAUL_DEFINE_DESTRUCT_MOVE_COPY(Monitor);

  class Impl;

  /**
   * @brief Get the name of the monitor, usually manufacturer and model number
   *
   * @return const char* string
   */
  [[nodiscard]] const char* getName() const noexcept;

  /**
   * @brief Get the size of the monitor
   *
   * @return Size in pixels
   */
  [[nodiscard]] Size getSize() const noexcept;

  /**
   * @brief Get the DPMM (dots/pixels per millimeter) of the monitor
   *
   * @return float in DPMM, defaultDPMM if unknown
   */
  [[nodiscard]] float getDPMM() const noexcept;

  /**
   * @brief Get the position of the monitor relative to the primary. Distance
   * from this monitor's top left pixel to the primary monitor's top left pixel.
   *
   * @return Position in pixels
   */
  [[nodiscard]] Position getPosition() const noexcept;

  // ColorSpace getColorSpace something about bit depth, gamma, LUTs, etc

  /**
   * @brief Check if this monitor is the primary monitor
   *
   * @return true if this monitor is the primary monitor, false otherwise
   */
  [[nodiscard]] bool isPrimary() const noexcept;

 private:
  /**
   * @brief Construct a new Monitor object
   *
   */
  Monitor() noexcept;
};

}  // namespace yaul

#endif /* _YAUL_MONITOR_HPP_ */