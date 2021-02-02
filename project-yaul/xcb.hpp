#ifndef YAUL_XCB_HPP
#define YAUL_XCB_HPP
#if defined(__linux) || defined(__linux__)

#include <yaul/common.hpp>

#include <X11/Xatom.h>
#include <X11/Xlib-xcb.h>
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <memory>

namespace yaul {

struct XCBInfo {
  xcb_connection_t* connection = nullptr;
  ::Display* display           = nullptr;
  Atom wmQuit                  = 0;
  int defaultScreen            = 0;

  /**
   * @brief Open connection to X server
   *
   * @return std::unique_ptr<XCBInfo>
   */
  static std::unique_ptr<XCBInfo> open() noexcept;

  XCBInfo() = default;

  ~XCBInfo() noexcept {
    if (display != nullptr)
      XCloseDisplay(display);
  }

  YAUL_NO_COPY(XCBInfo);
  YAUL_DEFINE_MOVE(XCBInfo);
};

extern const std::unique_ptr<XCBInfo> xcb;

/**
 * @class Wrapper class for XCB object that need to be freed after use
 *
 * @tparam T XCB object type
 */
template <typename T>
class XCBPtr {
 private:
  T* p;

 public:
  /**
   * @brief Construct a new XCBPtr object from a raw pointer returned by XCB
   *
   * @param p to take ownership of
   */
  explicit XCBPtr(T* p) noexcept : p(p) {}

  ~XCBPtr() noexcept { free(p); }  // NOLINT (cppcoreguidelines-no-malloc)

  /**
   * @brief Get the XCB object as a raw pointer
   *
   * @return T* XCB object
   */
  [[nodiscard]] inline T* get() const noexcept { return p; }

  YAUL_NO_COPY(XCBPtr);
  YAUL_NO_MOVE(XCBPtr);
};

}  // namespace yaul

#endif /* __linux__ */
#endif /* YAUL_XCB_HPP */