#include "xcb.hpp"

namespace yaul {

const std::unique_ptr<XCBInfo> xcb = XCBInfo::open();

std::unique_ptr<XCBInfo> XCBInfo::open() noexcept {
  std::unique_ptr<XCBInfo> info = std::make_unique<XCBInfo>();
  info->display                 = ::XOpenDisplay(nullptr);
  if (info->display == nullptr)
    return nullptr;

  info->defaultScreen = ::XDefaultScreen(info->display);

  info->connection = ::XGetXCBConnection(info->display);

  if (info->connection == nullptr)
    return nullptr;

  ::XSetEventQueueOwner(info->display, ::XCBOwnsEventQueue);

  // Set Atoms
  info->wmQuit = ::XInternAtom(info->display, "WM_QUIT", False);

  return info;
}

XCBInfo::XCBInfo(XCBInfo&& o) noexcept {
  std::swap(display, o.display);
  wmQuit        = o.wmQuit;
  defaultScreen = defaultScreen;
}

XCBInfo& XCBInfo::operator=(XCBInfo&& o) noexcept {
  if (this == &o)
    return *this;

  std::swap(display, o.display);
  wmQuit        = o.wmQuit;
  defaultScreen = defaultScreen;
  return *this;
}

}  // namespace yaul