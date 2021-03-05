#include "window_impl.hpp"

#include "common/logger.hpp"

namespace yaul {

// NOLINTNEXTLINE (cppcoreguidelines-avoid-non-const-global-variables)
std::unordered_map<xcb_window_t, Window::Impl*> Window::Impl::windowIDs;

void Window::Impl::createNativeWindow() noexcept(false) {
  const auto* xcb = XCB::instance();
  nativeWindow    = xcb_generate_id(xcb->connection());

  windowIDs.emplace(nativeWindow, this);

  // TODO (WattsUp) get visualID from frame buffer
  xcb_visualid_t visualID = xcb->screen()->root_visual;

  colormap = xcb_generate_id(xcb->connection());
  xcb_create_colormap(xcb->connection(), XCB_COLORMAP_ALLOC_NONE, colormap,
                      xcb->screen()->root, visualID);

  // clang-format off
  uint32_t eventMask =
      XCB_EVENT_MASK_KEY_PRESS |
      XCB_EVENT_MASK_KEY_RELEASE |
      XCB_EVENT_MASK_BUTTON_PRESS |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_BUTTON_MOTION |
      // XCB_EVENT_MASK_EXPOSURE | // Maximize button not producing an expose with proper size, configure notify is
      XCB_EVENT_MASK_VISIBILITY_CHANGE |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY |
      XCB_EVENT_MASK_FOCUS_CHANGE;
  // clang-format on
  const uint32_t valueMask             = XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;
  const std::array<uint32_t, 3> values = {eventMask, colormap, 0};

  xcb_create_window(xcb->connection(), XCB_COPY_FROM_PARENT, nativeWindow,
                    xcb->screen()->root, 0, 0, 1, 1, 0,
                    XCB_WINDOW_CLASS_COPY_FROM_PARENT, visualID, valueMask,
                    values.data());

  // Replace WM_PROTOCOLS with WM_DELETE_WINDOW to allow close window
  // interception
  auto wmProtocols    = xcb->atom(XCB::WM_PROTOCOLS);
  auto wmDeleteWindow = xcb->atom(XCB::WM_DELETE_WINDOW);
  xcb_change_property(xcb->connection(), XCB_PROP_MODE_REPLACE, nativeWindow,
                      wmProtocols, xcb->atom(XCB::ATOM), 32, 1,
                      &wmDeleteWindow);

  xcb_flush(xcb->connection());
}

void Window::Impl::processEvent(xcb_generic_event_t* event) noexcept {
  switch (event->response_type & (~0x80)) {
    case XCB_DESTROY_NOTIFY:
    case XCB_UNMAP_NOTIFY:
    case XCB_MAP_NOTIFY:
    case XCB_REPARENT_NOTIFY:
      break;
    case XCB_KEY_PRESS:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Key press");
      break;
    case XCB_KEY_RELEASE:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Key release");
      break;
    case XCB_BUTTON_PRESS:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Button press");
      break;
    case XCB_BUTTON_RELEASE:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Button release");
      break;
    case XCB_MOTION_NOTIFY:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Motion notify");
      break;
    case XCB_FOCUS_IN:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Focus in");
      break;
    case XCB_FOCUS_OUT:
      // TODO (WattsUp)
      Logger::instance().log(LogLevel::debug, "Focus out");
      break;
    case XCB_VISIBILITY_NOTIFY: {
      // TODO (WattsUp)
      auto* visibilityEvent = YAUL_XCB_EVENT(visibility_notify, event);
      Logger::instance().log(
          LogLevel::debug,
          "Visibility notify " + std::to_string(visibilityEvent->state));
    } break;
    case XCB_CONFIGURE_NOTIFY: {
      auto* configureEvent = YAUL_XCB_EVENT(configure_notify, event);
      auto itr             = windowIDs.find(configureEvent->window);
      if (itr == windowIDs.end()) {
        Logger::instance().log(LogLevel::error,
                               "Could not find Window::Impl for windowID");
        break;
      }
      auto* window = itr->second;

      window->lastSizeUpdate.width  = configureEvent->width;
      window->lastSizeUpdate.height = configureEvent->height;

      window->frameMarginsDirty = true;

      Logger::instance().log(LogLevel::debug,
                             "Configure " +
                                 std::to_string(configureEvent->width) + "x" +
                                 std::to_string(configureEvent->height) + " " +
                                 std::to_string(configureEvent->x) + "x" +
                                 std::to_string(configureEvent->y) + " " +
                                 std::to_string(configureEvent->border_width));
    } break;
    default:
      Logger::instance().log(
          LogLevel::warning,
          "Unknown XCB Event " + std::to_string(event->response_type));
      break;
  }
}

Edges Window::Impl::updateFrameMargins() noexcept {
  if (!frameMarginsDirty)
    return frameMargins;

  try {
    const auto* xcb = XCB::instance();

    if (xcb->atom(XCB::NET_FRAME_EXTENTS) != XCB_ATOM_NONE) {
      auto frameExtents = YAUL_XCB_REPLY(xcb, get_property, False, nativeWindow,
                                         xcb->atom(XCB::NET_FRAME_EXTENTS),
                                         xcb->atom(XCB::CARDINAL), 0, 4);
      if (frameExtents && frameExtents->type == XCB_ATOM_CARDINAL &&
          frameExtents->format == 32 && frameExtents->value_len == 4) {
        // NOLINTNEXTLINE (cppcoreguidelines-pro-type-reinterpret-cast)
        const auto* data = reinterpret_cast<uint32_t*>(
            xcb_get_property_value(frameExtents.get()));
        frameMargins.left   = *(data + 0);
        frameMargins.right  = *(data + 1);
        frameMargins.top    = *(data + 2);
        frameMargins.bottom = *(data + 3);
        frameMarginsDirty   = false;
        xcb_flush(xcb->connection());
        return frameMargins;
      }
    }

    // TODO (WattsUp) traverse the virtual roots tree to calculate the frame
    // margin

    xcb_flush(xcb->connection());
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }

  return frameMargins;
}

}  // namespace yaul