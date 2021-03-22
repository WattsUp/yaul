#include "window_mocked.hpp"

void WindowManager::processAsyncCalls() noexcept {
  while (!asyncQueue.empty()) {
    asyncQueue.front()();
    asyncQueue.pop_front();
  }
  asyncQueue.shrink_to_fit();
}