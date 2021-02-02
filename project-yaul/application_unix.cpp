#include "application_impl.hpp"

#include "common/logger.hpp"

#include <xcb/xcb.h>

namespace yaul {

void Application::Impl::loop() noexcept {
  threadReady = true;
  cv.notify_one();

  // TODO (WattsUp)

  // while (running) {
  //   ::XNextEvent(Window::Impl::xcbInfo.display, &event);
  //   if (event.type == ClientMessage &&
  //       static_cast<Atom>(event.xclient.data.l[0]) ==
  //       Window::Impl::xcbInfo.wmQuit) {
  //     Logger::instance().log(LogLevel::critical, "WM_QUIT was received");
  //     running = false;
  //     {
  //       std::lock_guard<std::mutex> lock(mutex);
  //       windows.clear();
  //     }
  //     cv.notify_one();
  //     return;
  //   }
  // }
}

void Application::Impl::stop() noexcept {
  running = false;

  // TODO (WattsUp)

  // Send a message to quit
  // XEvent event;
  // event.xclient.type      = ClientMessage;
  // event.xclient.data.l[0] = Window::Impl::xcbInfo.wmQuit;
  // ::XSendEvent(Window::Impl::xcbInfo.display, window, False, NoEventMask,
  // &event);

  if (thread->joinable())
    thread->join();
}

}  // namespace yaul