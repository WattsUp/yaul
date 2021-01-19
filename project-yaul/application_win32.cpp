#include "application_impl.hpp"

#include "common/logger.hpp"

#include <windows.h>

namespace yaul {

void Application::Impl::loop() noexcept {
  MSG msg;
  // Call PeekMessage once to identify this thread as a message processor
  ::PeekMessageW(&msg, nullptr, 0, 0, PM_NOREMOVE);
  threadReady = true;
  cv.notify_one();

  while (running) {
    if (::GetMessageW(&msg, nullptr, 0, 0) == 0) {
      Logger::instance().log(LogLevel::critical, "WM_QUIT was received");
      running = false;
      {
        std::lock_guard<std::mutex> lock(mutex);
        windows.clear();
      }
      cv.notify_one();
      return;
    }
    if (msg.message == YAUL_WM_NEW_WINDOW) {
      {
        std::lock_guard<std::mutex> lock(mutex);
        if (newWindowInfo == nullptr) {
          Logger::instance().log(
              LogLevel::error,
              "YAUL_WM_NEW_WINDOW was received with empty lParam");
          break;
        }

        try {
          Window window(newWindowInfo->size, "", Window::ShowState::hidden);
          newWindowInfo->createdWindow = window;
          windows.emplace(newWindowInfo->id, std::move(window));
        } catch (const std::exception& e) {
          newWindowInfo->result = Result(e.what());
        }
        newWindowInfo->complete = true;
        newWindowInfo           = nullptr;
      }
      cv.notify_one();
    } else if (msg.message == YAUL_WM_STOP_LOOP) {
      running = false;
      {
        std::lock_guard<std::mutex> lock(mutex);
        windows.clear();
      }
      cv.notify_one();
      return;
    } else {
      ::TranslateMessage(&msg);
      ::DispatchMessageW(&msg);
      auto itr = windows.begin();
      auto end = windows.end();
      while (itr != end) {
        auto& window = itr->second;
        if (window.shouldClose()) {
          {
            std::lock_guard<std::mutex> lock(mutex);
            window.setShowState(Window::ShowState::hidden);
            itr = windows.erase(itr);
          }
          cv.notify_one();
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