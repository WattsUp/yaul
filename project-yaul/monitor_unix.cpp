#include "monitor_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

std::list<Monitor::Impl> Monitor::Impl::enumerate() noexcept {
  std::list<Monitor::Impl> monitors;
  auto* screen = xcb_setup_roots_iterator(xcb_get_setup(xcb->connection)).data;

  auto* monitorInfos = xcb_randr_get_monitors_reply(
      xcb->connection,
      xcb_randr_get_monitors(xcb->connection, screen->root, False), nullptr);

  if (monitorInfos == nullptr)
    return monitors;

  // auto num  = xcb_randr_get_monitors_monitors_length(monitorInfos);
  auto iter = xcb_randr_get_monitors_monitors_iterator(monitorInfos);
  while (iter.rem != 0) {
    monitors.emplace_back(Monitor::Impl(screen, iter.data));

    xcb_randr_monitor_info_next(&iter);
  }
  return monitors;
}

Monitor::Impl::Impl(xcb_screen_t* screen,
                    xcb_randr_monitor_info_t* monitorInfo) noexcept
    : screen(screen) {
  XCBPtr<xcb_get_atom_name_reply_t> nameReply(xcb_get_atom_name_reply(
      xcb->connection, xcb_get_atom_name(xcb->connection, monitorInfo->name),
      nullptr));
  name = string(xcb_get_atom_name_name(nameReply.get()),
                xcb_get_atom_name_name_length(nameReply.get()));

  size.width  = static_cast<int>(monitorInfo->width);
  size.height = static_cast<int>(monitorInfo->height);

  position.x = static_cast<int>(monitorInfo->x);
  position.y = static_cast<int>(monitorInfo->y);

  dpmm = static_cast<float>(monitorInfo->width) /
         static_cast<float>(monitorInfo->width_in_millimeters);

  primary = monitorInfo->primary == True;
}

}  // namespace yaul