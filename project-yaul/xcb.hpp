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

    MOTIF_WM_HINTS,

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

    NET_WM_WINDOW_TYPE,
    NET_WM_WINDOW_TYPE_DESKTOP,
    NET_WM_WINDOW_TYPE_DOCK,
    NET_WM_WINDOW_TYPE_TOOLBAR,
    NET_WM_WINDOW_TYPE_MENU,
    NET_WM_WINDOW_TYPE_UTILITY,
    NET_WM_WINDOW_TYPE_SPLASH,
    NET_WM_WINDOW_TYPE_DIALOG,
    NET_WM_WINDOW_TYPE_DROPDOWN_MENU,
    NET_WM_WINDOW_TYPE_POPUP_MENU,
    NET_WM_WINDOW_TYPE_TOOLTIP,
    NET_WM_WINDOW_TYPE_NOTIFICATION,
    NET_WM_WINDOW_TYPE_COMBO,
    NET_WM_WINDOW_TYPE_DND,
    NET_WM_WINDOW_TYPE_NORMAL,

    NET_FRAME_EXTENTS,

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
  [[nodiscard]] inline xcb_atom_t atom(Atom atom) const noexcept {
    return atoms.at(static_cast<size_t>(atom));
  }

  /**
   * @brief Set window manager hints for a window
   *
   * @param window to configure
   * @param decorations true will add title, and buttons, false will remove
   * decorations
   * @param resizable true will add resizing handles, false will not
   */
  void setWMHints(xcb_window_t window,
                  bool decorations,
                  bool resizable) const noexcept;

  enum class WMState : uint8_t {
    hidden,
    restore,
    maximize,
    minimize,
    fullscreen,
    onTop,
    onBottom,
    attention
  };

  /**
   * @brief Set window manager state for a window
   *
   * @param window to configure
   * @param set true will set the state, false will reset the state
   * @param state to change
   */
  void setWMState(xcb_window_t window, bool set, WMState state) const noexcept;

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

  struct MotifHints {
    enum class Flags : uint32_t { functions = 1 << 0, decorations = 1 << 1 };
    enum class Functions : uint32_t {
      none     = 0,
      all      = 1 << 0,
      resize   = 1 << 1,
      move     = 1 << 2,
      minimize = 1 << 3,
      maximize = 1 << 4,
      close    = 1 << 5
    };
    enum class Decorations : uint32_t {
      none     = 0,
      all      = 1 << 0,
      border   = 1 << 1,
      resizeH  = 1 << 2,
      title    = 1 << 3,
      menu     = 1 << 4,
      minimize = 1 << 5,
      maximize = 1 << 6
    };

    friend inline Flags operator|(const Flags& a, const Flags& b) noexcept {
      return static_cast<Flags>(static_cast<uint32_t>(a) |
                                static_cast<uint32_t>(b));
    }

    friend inline Flags operator&(const Flags& a, const Flags& b) noexcept {
      return static_cast<Flags>(static_cast<uint32_t>(a) &
                                static_cast<uint32_t>(b));
    }

    friend inline Flags& operator|=(Flags& a, const Flags& b) noexcept {
      return a = a | b;
    }

    friend inline Functions operator|(const Functions& a,
                                      const Functions& b) noexcept {
      return static_cast<Functions>(static_cast<uint32_t>(a) |
                                    static_cast<uint32_t>(b));
    }

    friend inline Functions operator&(const Functions& a,
                                      const Functions& b) noexcept {
      return static_cast<Functions>(static_cast<uint32_t>(a) &
                                    static_cast<uint32_t>(b));
    }

    friend inline Functions& operator|=(Functions& a,
                                        const Functions& b) noexcept {
      return a = a | b;
    }

    friend inline Decorations operator|(const Decorations& a,
                                        const Decorations& b) noexcept {
      return static_cast<Decorations>(static_cast<uint32_t>(a) |
                                      static_cast<uint32_t>(b));
    }

    friend inline Decorations operator&(const Decorations& a,
                                        const Decorations& b) noexcept {
      return static_cast<Decorations>(static_cast<uint32_t>(a) &
                                      static_cast<uint32_t>(b));
    }

    friend inline Decorations& operator|=(Decorations& a,
                                          const Decorations& b) noexcept {
      return a = a | b;
    }

    Flags flags;
    Functions functions;
    Decorations decorations;
    int32_t input_mode;
    uint32_t status;
  };
  static constexpr uint32_t MotifHintsSize = sizeof(MotifHints) / 4;
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
#define YAUL_XCB_REPLY(xcb, call, ...)                                   \
  XCBPtr<xcb_##call##_reply_t>(xcb_##call##_reply(                       \
      (xcb)->connection(), xcb_##call((xcb)->connection(), __VA_ARGS__), \
      nullptr))

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_XCB_EVENT(type, genericEvent) \
  reinterpret_cast<xcb_##type##_event_t*>(genericEvent);  // NOLINT

// NOLINTNEXTLINE (cppcoreguidelines-macro-usage)
#define YAUL_XCB_SEND(xcb, window, mask, event)        \
  xcb_send_event((xcb)->connection(), 0, window, mask, \
                 reinterpret_cast<const char*>(&event));  // NOLINT

}  // namespace yaul

#endif /* __linux__ */
#endif /* YAUL_XCB_HPP */