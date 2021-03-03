#include "xcb.hpp"

namespace yaul {

const std::array<const char*, XCB::ATOMS_LENGTH> atomStrings = {
    "WM_QUIT",
    "WM_DELETE_WINDOW",
    "WM_PROTOCOLS",

    "WM_CHANGE_STATE",

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
        data.connection, 0, static_cast<uint16_t>(strlen(atomStrings.at(i))),
        atomStrings.at(i));
  }

  for (size_t i = 0; i < ATOMS_LENGTH; ++i) {
    XCBPtr<xcb_intern_atom_reply_t> reply(
        xcb_intern_atom_reply(data.connection, cookies.at(i), nullptr));
    atoms.at(i) = reply->atom;
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

}  // namespace yaul