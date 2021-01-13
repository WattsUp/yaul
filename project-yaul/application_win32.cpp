#include "application_impl.hpp"

#include "common/logger.hpp"

#include <windows.h>

namespace yaul {

Window* Application::Impl::addWindow(
    const char* id,
    Size size,
    Window::ShowState showState) noexcept(false) {
  // Send new window info to loop
  NewWindowInfo windowInfo;
  windowInfo.size      = size;
  windowInfo.showState = showState;
  windowInfo.id        = id;

  // Wait for loop to become a message processing thread
  std::unique_lock<std::mutex> lock(mutex);
  cv.wait(lock, [this] { return static_cast<bool>(threadReady); });
  newWindowInfo = &windowInfo;

  // Send new window message
  if (::PostThreadMessageW(::GetThreadId(thread->native_handle()),
                           YAUL_WM_NEW_WINDOW, 0, 0) == 0) {
    throw std::exception("Failed to post new window message");
  }

  // Wait for loop to create window
  // windowInfo.complete is true once consumed
  cv.wait(lock, [&windowInfo] { return windowInfo.complete; });

  if (windowInfo.result.failed()) {
    throw std::exception(windowInfo.result);
  }

  return windowInfo.createdWindow;
}

void Application::Impl::loop() noexcept {
  MSG msg;
  // Call PeekMessage once to identify this thread as a message processor
  ::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE);
  threadReady = true;
  cv.notify_all();

  while (running) {
    if (::GetMessageW(&msg, nullptr, 0, 0) == 0) {
      Logger::instance().log(LogLevel::critical, "WM_QUIT was received");
      running = false;
      windows.clear();
      cv.notify_all();
      return;
    }
    if (msg.message == YAUL_WM_NEW_WINDOW) {
      std::lock_guard<std::mutex> lock(mutex);
      if (newWindowInfo == nullptr) {
        Logger::instance().log(
            LogLevel::error,
            "YAUL_WM_NEW_WINDOW was received with empty lParam");
        break;
      }

      try {
        std::unique_ptr<Window> window(Window::createWindow(
            newWindowInfo->size, newWindowInfo->id, newWindowInfo->showState));
        newWindowInfo->createdWindow = window.get();
        windows.emplace_back(std::move(window));
      } catch (const std::exception& e) {
        newWindowInfo->result = Result(e.what());
      }
      newWindowInfo->complete = true;
      newWindowInfo           = nullptr;
      cv.notify_all();
    } else if (msg.message == YAUL_WM_STOP_LOOP) {
      running = false;
      windows.clear();
      cv.notify_all();
      return;
    } else {
      ::TranslateMessage(&msg);
      ::DispatchMessageW(&msg);
      auto itr = windows.begin();
      auto end = windows.end();
      while (itr != end) {
        const auto& window = *itr;
        if (window->shouldClose()) {
          window->setShowState(Window::ShowState::hidden);
          itr = windows.erase(itr);
          cv.notify_all();
        } else {
          ++itr;
        }
      }
    }
  }
}

void Application::Impl::stop() noexcept {
  running = false;

  // Send a message to refresh GetMessage
  if (::PostThreadMessageW(::GetThreadId(thread->native_handle()),
                           YAUL_WM_STOP_LOOP, 0, 0) == 0) {
    ::PostQuitMessage(-1);
  }

  if (thread->joinable()) {
    thread->join();
  }
}

}  // namespace yaul