#include "monitor_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

std::list<Monitor::Impl> Monitor::Impl::enumerate() noexcept {
  std::list<Monitor::Impl> monitors;
  const XCB* xcb = nullptr;
  try {
    xcb = XCB::instance();
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
    return monitors;
  }

  auto* monitorInfos = xcb_randr_get_monitors_reply(
      xcb->connection(),
      xcb_randr_get_monitors(xcb->connection(), xcb->screen()->root, False),
      nullptr);

  if (monitorInfos == nullptr) {
    xcb_flush(xcb->connection());
    return monitors;
  }

  // auto num  = xcb_randr_get_monitors_monitors_length(monitorInfos);
  auto iter = xcb_randr_get_monitors_monitors_iterator(monitorInfos);
  while (iter.rem != 0) {
    monitors.emplace_back(Monitor::Impl(iter.data));

    xcb_randr_monitor_info_next(&iter);
  }
  xcb_flush(xcb->connection());
  return monitors;
}

Monitor::Impl::Impl(xcb_randr_monitor_info_t* monitorInfo) noexcept {
  try {
    const auto* xcb = XCB::instance();
    XCBPtr<xcb_get_atom_name_reply_t> nameReply(xcb_get_atom_name_reply(
        xcb->connection(),
        xcb_get_atom_name(xcb->connection(), monitorInfo->name), nullptr));
    name = string(xcb_get_atom_name_name(nameReply.get()),
                  xcb_get_atom_name_name_length(nameReply.get()));
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }

  size.width  = static_cast<int>(monitorInfo->width);
  size.height = static_cast<int>(monitorInfo->height);

  position.x = static_cast<int>(monitorInfo->x);
  position.y = static_cast<int>(monitorInfo->y);

  dpmm = static_cast<float>(monitorInfo->width) /
         static_cast<float>(monitorInfo->width_in_millimeters);

  primary = monitorInfo->primary == True;
}

}  // namespace yaul