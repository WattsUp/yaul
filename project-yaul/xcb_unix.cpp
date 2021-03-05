#include "xcb.hpp"

#include <list>

namespace yaul {

const std::array<const char*, XCB::ATOMS_LENGTH> atomStrings = {
    "WM_QUIT",
    "WM_DELETE_WINDOW",
    "WM_PROTOCOLS",

    "WM_CHANGE_STATE",

    "_MOTIF_WM_HINTS",

    "_NET_SUPPORTED",

    "_NET_WM_NAME",
    "_NET_WM_ICON_NAME",
    "_NET_WM_ICON",

    "_NET_WM_STATE",
    "_NET_WM_STATE_ABOVE",
    "_NET_WM_STATE_BELOW",
    "_NET_WM_STATE_FULLSCREEN",
    "_NET_WM_STATE_MAXIMIZED_HORZ",
    "_NET_WM_STATE_MAXIMIZED_VERT",
    "_NET_WM_STATE_MODAL",
    "_NET_WM_STATE_STAYS_ON_TOP",
    "_NET_WM_STATE_DEMANDS_ATTENTION",
    "_NET_WM_STATE_HIDDEN",

    "_NET_WM_WINDOW_TYPE",
    "_NET_WM_WINDOW_TYPE_DESKTOP",
    "_NET_WM_WINDOW_TYPE_DOCK",
    "_NET_WM_WINDOW_TYPE_TOOLBAR",
    "_NET_WM_WINDOW_TYPE_MENU",
    "_NET_WM_WINDOW_TYPE_UTILITY",
    "_NET_WM_WINDOW_TYPE_SPLASH",
    "_NET_WM_WINDOW_TYPE_DIALOG",
    "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU",
    "_NET_WM_WINDOW_TYPE_POPUP_MENU",
    "_NET_WM_WINDOW_TYPE_TOOLTIP",
    "_NET_WM_WINDOW_TYPE_NOTIFICATION",
    "_NET_WM_WINDOW_TYPE_COMBO",
    "_NET_WM_WINDOW_TYPE_DND",
    "_NET_WM_WINDOW_TYPE_NORMAL",

    "_NET_FRAME_EXTENTS",

    "ATOM",
    "TEXT",
    "UTF8_STRING",
    "CARDINAL",
};

XCB::XCB() noexcept(false) {
  data.display = ::XOpenDisplay(nullptr);
  if (data.display == nullptr)
    throw std::runtime_error("XCB display not opened");

  data.defaultScreen = ::XDefaultScreen(data.display);

  data.connection = ::XGetXCBConnection(data.display);

  if (data.connection == nullptr)
    throw std::runtime_error("XCB connection not opened");

  ::XSetEventQueueOwner(data.display, ::XCBOwnsEventQueue);

  data.screen = xcb_setup_roots_iterator(xcb_get_setup(data.connection)).data;

  const uint32_t mask                  = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  const std::array<uint32_t, 2> values = {data.screen->white_pixel,
                                          XCB_EVENT_MASK_EXPOSURE};

  data.dummyWindow = xcb_generate_id(data.connection);
  xcb_create_window(data.connection, XCB_COPY_FROM_PARENT, data.dummyWindow,
                    data.screen->root, 0, 0, 1, 1, 0,
                    XCB_WINDOW_CLASS_COPY_FROM_PARENT, data.screen->root_visual,
                    mask, values.data());

  // Set Atoms
  std::array<xcb_intern_atom_cookie_t, ATOMS_LENGTH> cookies{};
  for (size_t i = 0; i < ATOMS_LENGTH; ++i) {
    cookies.at(i) = xcb_intern_atom(
        data.connection, True, static_cast<uint16_t>(strlen(atomStrings.at(i))),
        atomStrings.at(i));
  }

  for (size_t i = 0; i < ATOMS_LENGTH; ++i) {
    XCBPtr<xcb_intern_atom_reply_t> reply(
        xcb_intern_atom_reply(data.connection, cookies.at(i), nullptr));
    atoms.at(i) = reply->atom;
    if (reply->atom == XCB_ATOM_NONE) {
      Logger::instance().log(
          LogLevel::warning,
          "Atom does not already exist " + std::string(atomStrings.at(i)));
    }
  }
}

XCB::~XCB() noexcept {
  xcb_destroy_window(data.connection, data.dummyWindow);
  ::XCloseDisplay(data.display);
}

const XCB* XCB::instance() noexcept(false) {
  static XCB instance;
  if (instance.data.connection == nullptr)
    throw std::runtime_error("XCB connection not opened");
  return &instance;
}

void XCB::setWMHints(xcb_window_t window,
                     bool decorations,
                     bool resizable) const noexcept {
  MotifHints hints{};
  hints.flags = MotifHints::Flags::functions | MotifHints::Flags::decorations;
  hints.functions = MotifHints::Functions::move |
                    MotifHints::Functions::minimize |
                    MotifHints::Functions::close;
  hints.decorations = MotifHints::Decorations::none;
  hints.input_mode  = 0;
  hints.status      = 0;

  if (decorations) {
    hints.decorations |= MotifHints::Decorations::title |
                         MotifHints::Decorations::minimize |
                         MotifHints::Decorations::menu;
    if (resizable) {
      hints.decorations |= MotifHints::Decorations::border |
                           XCB::MotifHints::Decorations::maximize |
                           MotifHints::Decorations::resizeH;
      hints.functions |=
          MotifHints::Functions::maximize | MotifHints::Functions::resize;
    }
  } else if (resizable) {
    hints.decorations |= MotifHints::Decorations::border;
    hints.functions |=
        MotifHints::Functions::maximize | MotifHints::Functions::resize;
  }
  xcb_change_property(data.connection, XCB_PROP_MODE_REPLACE, window,
                      atom(MOTIF_WM_HINTS), atom(MOTIF_WM_HINTS), 32,
                      MotifHintsSize, &hints);

  xcb_flush(data.connection);
}

void XCB::setWMState(xcb_window_t window,
                     bool set,
                     WMState state) const noexcept {
  xcb_client_message_event_t event;
  event.response_type  = XCB_CLIENT_MESSAGE;
  event.format         = 32;
  event.sequence       = 0;
  event.window         = window;
  event.type           = atom(NET_WM_STATE);
  event.data.data32[0] = set ? 1 : 0;
  event.data.data32[1] = 0;
  event.data.data32[2] = 0;
  event.data.data32[3] = 0;
  event.data.data32[4] = 0;

  switch (state) {
    case WMState::hidden:
    case WMState::restore:
    case WMState::minimize:
      break;
    case WMState::maximize:
      event.data.data32[1] = atom(NET_WM_STATE_MAXIMIZED_HORZ);
      event.data.data32[2] = atom(NET_WM_STATE_MAXIMIZED_VERT);
      break;
    case WMState::fullscreen:
      event.data.data32[1] = atom(NET_WM_STATE_FULLSCREEN);
      break;
    case WMState::onTop:
      event.data.data32[1] = atom(NET_WM_STATE_ABOVE);
      event.data.data32[2] = atom(NET_WM_STATE_STAYS_ON_TOP);
      break;
    case WMState::onBottom:
      event.data.data32[1] = atom(NET_WM_STATE_BELOW);
      break;
    case WMState::attention:
      event.data.data32[1] = atom(NET_WM_STATE_DEMANDS_ATTENTION);
      break;
  }

  YAUL_XCB_SEND(
      this, screen()->root,
      XCB_EVENT_MASK_STRUCTURE_NOTIFY | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,
      event);
  xcb_flush(data.connection);
}

}  // namespace yaul