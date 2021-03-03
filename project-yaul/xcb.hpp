#ifndef YAUL_XCB_HPP
#define YAUL_XCB_HPP
#if defined(__linux) || defined(__linux__)

#include "common/logger.hpp"

#include <yaul/common.hpp>
#include <yaul/dimensions.hpp>

#include <X11/Xatom.h>
#include <X11/Xlib-xcb.h>
#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include <array>
#include <memory>
#include <vector>

namespace yaul {

class XCB {
 public:
  YAUL_DEFINE_DESTRUCT(XCB);
  YAUL_NO_COPY(XCB);
  YAUL_NO_MOVE(XCB);

  [[nodiscard]] inline xcb_connection_t* connection() const noexcept {
    return data.connection;
  }

  [[nodiscard]] inline ::Display* display() const noexcept {
    return data.display;
  }

  [[nodiscard]] inline xcb_screen_t* screen() const noexcept {
    return data.screen;
  }

  [[nodiscard]] inline xcb_window_t dummyWindow() const noexcept {
    return data.dummyWindow;
  }

  [[nodiscard]] inline int defaultScreen() const noexcept {
    return data.defaultScreen;
  }

  enum Atom : size_t {
    WM_QUIT,
    WM_DELETE_WINDOW,
    WM_PROTOCOLS,

    WM_CHANGE_STATE,

    NET_SUPPORTED,

    NET_WM_NAME,
    NET_WM_ICON_NAME,
    NET_WM_ICON,

    NET_WM_STATE,
    NET_WM_STATE_ABOVE,
    NET_WM_STATE_BELOW,
    NET_WM_STATE_FULLSCREEN,
    NET_WM_STATE_MAXIMIZED_HORZ,
    NET_WM_STATE_MAXIMIZED_VERT,
    NET_WM_STATE_MODAL,
    NET_WM_STATE_STAYS_ON_TOP,
    NET_WM_STATE_DEMANDS_ATTENTION,
    NET_WM_STATE_HIDDEN,

    ATOM,
    TEXT,
    UTF8_STRING,
    CARDINAL,

    ATOMS_LENGTH
  };

  /**
   * @brief Get a XCB atom
   *
   * @param atom to lookup
   * @return xcb_atom_t
   */
  [[nodiscard]] inline xcb_atom_t atom(Atom atom) const {
    return atoms.at(static_cast<size_t>(atom));
  }

  /**
   * @brief Get the singleton XCB, throw an error if failed to open
   * connection
   *
   * @return const XCB*
   * @throw std::exception when XCB is not initialized
   */
  [[nodiscard]] static const XCB* instance() noexcept(false);

 private:
  /**
   * @brief Construct a new XCB object by opening X display and xcb connection
   * then creating atoms and the like
   *
   * @throw std::exception if any initialization operation fails
   */
  XCB() noexcept(false);

  std::array<xcb_atom_t, ATOMS_LENGTH> atoms{};

  struct {
    xcb_connection_t* connection = nullptr;
    ::Display* display           = nullptr;
    xcb_screen_t* screen         = nullptr;
    xcb_window_t dummyWindow     = 0;
    int defaultScreen            = 0;
  } data;
};

struct XCBPtrDeleter {
  void operator()(void* p) const noexcept {
    std::free(p);  // NOLINT (cppcoreguidelines-no-malloc)
  }
};

/**
 * @class Wrapper class for XCB object that need to be freed after use
 *
 * @tparam T XCB object type
 */
template <typename T>
using XCBPtr = std::unique_ptr<T, XCBPtrDeleter>;

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_XCB_REPLY(call, connection, ...) \
  XCBPtr<call##_reply_t>(                     \
      call##_reply(connection, call(connection, __VA_ARGS__), nullptr))

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_XCB_EVENT(type, genericEvent) \
  reinterpret_cast<xcb_##type##_event_t*>(genericEvent);  // NOLINT

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_XCB_SEND(xcb, window, mask, event)                       \
  xcb_send_event((xcb)->connection(), 0, (xcb)->screen()->root, mask, \
                 reinterpret_cast<const char*>(&event));  // NOLINT

}  // namespace yaul

#endif /* __linux__ */
#endif /* YAUL_XCB_HPP */