#include "application_impl.hpp"

#include "common/logger.hpp"
#include "xcb.hpp"

namespace yaul {

void Application::Impl::loop() noexcept {
  threadReady = true;
  cv.notify_one();

  try {
    const auto* xcb = XCB::instance();
    xcb_flush(xcb->connection());

    while (running) {
      XCBPtr<xcb_generic_event_t> event(xcb_wait_for_event(xcb->connection()));
      if (event.get() == nullptr)
        break;
      if ((event->response_type & (~0x80)) == XCB_CLIENT_MESSAGE) {
        auto* clientMessage = YAUL_XCB_EVENT(client_message, event.get());
        Logger::instance().log(
            LogLevel::debug,
            "Client message " + std::to_string(clientMessage->type));
        if (clientMessage->type == xcb->atom(XCB::WM_QUIT)) {
          running = false;
          std::lock_guard<std::mutex> lock(mutex);
          windows.clear();
          cv.notify_one();
          return;
        }
        if (clientMessage->type == xcb->atom(XCB::WM_PROTOCOLS) &&
            clientMessage->data.data32[0] == xcb->atom(XCB::WM_DELETE_WINDOW)) {
          // Close window
          auto itr = windows.begin();
          auto end = windows.end();
          while (itr != end) {
            auto& window = itr->second;
            if (*window.impl<Window::Impl>() == clientMessage->window) {
              std::lock_guard<std::mutex> lock(mutex);
              window.setShowState(Window::ShowState::hidden);
              windows.erase(itr);
              cv.notify_one();
              break;
            }
            ++itr;
          }
        }
      } else {
        Window::Impl::processEvent(event.get());
      }
    }
  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
  }
}

void Application::Impl::stop() noexcept {
  running = false;

  try {
    const auto* xcb = XCB::instance();

    xcb_client_message_event_t event;
    event.response_type  = XCB_CLIENT_MESSAGE;
    event.format         = 32;
    event.sequence       = 0;
    event.window         = xcb->dummyWindow();
    event.type           = xcb->atom(XCB::WM_QUIT);
    event.data.data32[0] = 0;

    YAUL_XCB_SEND(xcb, xcb->dummyWindow(), XCB_EVENT_MASK_NO_EVENT, event);
    xcb_flush(xcb->connection());

  } catch (const std::exception& e) {
    Logger::instance().log(LogLevel::error, e.what());
    return;
  }

  if (thread->joinable())
    thread->join();
}

}  // namespace yaul